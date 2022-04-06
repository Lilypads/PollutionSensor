#include "neo6m.h"

NEO6M::NEO6M(neo6mSettings theseSettings){
settings = theseSettings;
}

void NEO6M::startMeasurement(){
#ifdef DEBUG
    fprintf(stderr,"starting GPS acquisition...\n")
#endif
//spawn the acquisition thread

if (nullptr != daqThread) {
    // already running
#ifdef DEBUG
    fprintf(stderr,"starting GPS acquisition...\n")
#endif
return;
}

//start the polling thread
daqThread = new std::thread(execPollingThread,this);
};

void NEO6M::stopMeasurement(){
isPollingUart = false;
//here we just wait untill the daq thread stops
if (daqThread != nullptr){
    daqThread->join();
    delete daqThread;
    daqThread = nullptr;
    }
close(fd);
};

void NEO6M::pollUartDev(){
    char circBuff[NMEA_MAX_SENTENCE_SIZE*2]; //this way we can fit at least one full message in regardless of where we start
    char data2Send[NMEA_MAX_SENTENCE_SIZE];
    // not that i think the raspberry pi is 64 bit but i dont think my lsp know this
    char* circBuffPtr;
    uint16_t rbHead=0; //this is the most recent data element
    uint16_t rbIdxLast = sizeof(circBuff); //
    uint16_t rbRemainingVerticalSpace;
    int rbLatestStartChar =-1; //int so we can invalidate it

    fd = open(settings.serialDevice.c_str(), O_RDONLY | O_NOCTTY | O_NDELAY);
    configurePort(fd, settings.baudrate);
    isPollingUart = true;
    int nbytes = 0;
    int scanDataLen;
    uint buffDataLen; //how many bytes total are there in the buffer
    uint lenFirstPart;
    uint lenSecondPart;
    uint idxLastNewElement;

    while(isPollingUart){
        // this is a sort of quasi ring buffer where we read as much data into
        // the buffer beffore it wraps since read() can't write direcly into a
        // ring buffer we just point it to the current next free element and
        // ask for however many verticle elements are left before wrapping

        //decide how many more bytes we can cram into our buffer
        rbRemainingVerticalSpace = rbIdxLast-rbHead;
        rbHead%=sizeof(circBuff);//wrap the data start pointer
        // read the byte into the buffer
        nbytes = read(fd,circBuffPtr+rbHead,rbRemainingVerticalSpace);
        if(nbytes<=0){
            fprintf(stderr, "failed to read from port, read() returned %d\n", nbytes);
        }
        //scan the HEAD through the new bytes to see if we recieved a termination character
        // or a sentance start character
        //note this will never wrap because we only ever ask for the remaining
        //"vertical space" worth of bytes from read
        idxLastNewElement = rbHead+nbytes;
        while (rbHead<idxLastNewElement) {
           // first we track if we encounter any message starts
           if (circBuff[rbHead]==NMEA_SENTENCE_START_DELIM){
               // we only ever need to track one of these because there should only be one per sentence
               if (rbLatestStartChar>0){
                   fprintf(stderr, "Detected multiple sentace starts without a sentance Termiation, will Use Latest and disregard the rest\n");
               }
               rbLatestStartChar = rbHead;

           }
           // we detected the termination character
           if (circBuff[rbHead]==NMEA_SENTENCE_END_DELIM){
               // calculate length of scanned data
               scanDataLen = rbHead - rbLatestStartChar;
               buffDataLen = rbHead - rbLatestStartChar;
               if(rbLatestStartChar<0){
                   fprintf(stderr, "Error in neo6m::pollUartDev(): Inclomplete NMEA message Detected, disregarding...\n");

               }
               // here we need to decide
               if (scanDataLen<0){
                   scanDataLen = rbHead + sizeof(circBuff)-rbLatestStartChar;
                   lenFirstPart = sizeof(circBuff)- rbLatestStartChar;
                   lenSecondPart = rbHead; // compiler will optimise this one away

                   memcpy(data2Send, circBuff+rbLatestStartChar,lenFirstPart);
                   memcpy(data2Send+lenFirstPart, circBuff, lenSecondPart);
                }
               else{
                   memcpy(data2Send, circBuff+rbLatestStartChar, scanDataLen);
               }
               if (scanDataLen>NMEA_MAX_SENTENCE_SIZE){
                   fprintf(stderr, "Warning: NMEA sentence length greater than Expected. message corruption may occur! (%d)",scanDataLen);
               }
               //now we have to reconstitute the ringbufferised sentence into the right order
               hasNmeaSentance(data2Send, scanDataLen);
               //un set the latest sentance start flag
               rbLatestStartChar = -1;
           }
       }
    }
};

int NEO6M::configurePort(int fd, int baud){
// adapted from:
// https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
      struct termios tty;
        if (tcgetattr (fd, &tty) != 0)
        {
                fprintf(stderr,"error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed(&tty, baud);
        cfsetispeed(&tty, baud);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        /*
        * Im setting vmin to 1 here which means we read out bytes 1 at a
        * time which is inefficient but there is no other way to make
        * this code "real time" since the messages all have variable size
        * and come in random order so if you were buffering bytes in the
        * device, there is always a chance that the end of the message
        * doesn't get to the end of the buffer so then you need to wait
        * untill the next one comes allong before the user gets the
        * callback
            */
        tty.c_cc[VMIN]  = 1;            // set to blocking buffer at least 1 char
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= 0; //parity bit
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
               fprintf(stderr, "Failed to configure port");
                return -1;
        }
        return 0;
};

bool NEO6M::testChecksum(char* sentance){
// largely adapted from:
  // https://github.com/craigpeacock/NMEA-GPS/blob/23f15158f124cc2b65c0468d5c690be23a449a77/gps.c#L109
	char *checksum_str;
	int returned_checksum;
	int calculated_checksum = 0;

	// Checksum is postcede by *
	checksum_str = strchr(sentance, NMEA_SENTENCE_CHECKSUM_DELIM);
	if (checksum_str != NULL){
		// Remove checksum from sentance
		*checksum_str = '\0';

		// loop through remaining sentance to Calculate checksum, starting after $ (i = 1)
		for (int i = 1; i < strlen(sentance); i++) {
			calculated_checksum = calculated_checksum ^ sentance[i];
		}

        //returned checksum is apparently in hex characters
		returned_checksum = NEO6M::hexChar2Int((char *)checksum_str+1);
		if (returned_checksum == calculated_checksum) {
			//printf("Checksum OK");
			return 1;
		}
	} else {
#ifdef DEBUG
		fprintf(stderr,"Error: Checksum missing or NULL NMEA message\r\n");
#endif
		return false;
	}
	return 0;
};

int NEO6M::hexChar2Int(char* checksumChar){
    // TODO write a test for this method should be fairly easy!!
    int checksum;
    // convert the msb and lsb into ints and then scale msb by a factor of 16
    checksum = hexChar2IntLUT[(uint)(*checksumChar)]*16+hexChar2IntLUT[(uint)(*(checksumChar+1))];
    return checksum;
};
