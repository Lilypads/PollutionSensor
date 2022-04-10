#include "neo6m.h"

int hexChar2IntLUT[] = {
    // ASCII
    // 0  1   2   3   4   5   6   7   7   8   10 11  12  13  14  15
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0-15
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 16-31
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 32-47
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  -1, -1, -1, -1, -1, -1, // 48-63
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 64-79
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 80-95
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 96-111
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 112-127
};

int decChar2IntLUT[] =
    {
// ASCII
//0  1   2   3   4   5   6   7   7   8   10 11  12  13  14  15
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0-15
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 16-31
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 32-47
 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, // 48-63
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 64-79
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 80-95
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 96-111
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 112-127
};

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

void NEO6M::pollUartDev() {
  isPollingUart = true;

  uint buffsize = NMEA_MAX_SENTENCE_SIZE * 2;
  char buff[NMEA_MAX_SENTENCE_SIZE *2]; // this way we can fit at least one full message in regardless
                // of where we start
  char data2Send[NMEA_MAX_SENTENCE_SIZE];
  int nbytes;
  parsedNmeaSent parsedSent;

  fd = open(settings.serialDevice.c_str(), O_RDONLY | O_NOCTTY | O_NDELAY);
  configurePort(fd, settings.baudrate);
  tcflush(fd, TCIFLUSH); // BF flush the buffered data, we care only for the

  while (isPollingUart) {
    nbytes = read(fd, &buff, buffsize);
    if (nbytes <= 0) {
      perror("Failed to read from port, read() returned");
      // exit(1);
    }

    if (parseNmeaStr(buff, nbytes, parsedSent) < 0) {
      fprintf(stderr,"Failed to parse NMEA Buffer: %.*s\n",nbytes,buff);
    }
    else {//if the parse operation was successfull, we can expect the nmeaSentance Properties to be
    // do nothing
    }

  } //end of polling
}

int NEO6M::parseNmeaStr(char* thisSent, int  size, parsedNmeaSent& outputSent) // TODO void pollUartDev(); // TODO
{
    //clear
    memset(&outputSent,0,sizeof(parsedNmeaSent));
    //check if the first char is indeed a start char if not then this is an incomplete message and we will return
    if (*thisSent != NMEA_SENTENCE_START_DELIM){
        fprintf(stderr,"Sentance Incomplete: %.*s\n",size,thisSent);
        return(-1);
    }

    //the first 6 chars are always start char and message type
    //the last
    int idxChecksumStart = size-1-1-NMEA_END_OF_SENT_lENGTH-1;
    if (*(thisSent+idxChecksumStart) != NMEA_SENTENCE_CHECKSUM_DELIM){
        fprintf(stderr,"NMEA Checksum Delim Missing: \"%.*s\" we found: %c at possition %i\n",size,thisSent,*(thisSent-idxChecksumStart),idxChecksumStart);
        return(-2);
    }

    //test checksum
    if (testChecksum(thisSent)<0){
        fprintf(stderr,"Checksum Failed!\n");
        // return(-1);
    };

    // trimCheckSum
    *(thisSent + idxChecksumStart) = '\0';
    // loop through the array and track what part of the message we are in
    int i=0;
    char* nextSent;
    char* checksum;
    checksum = thisSent+idxChecksumStart;
    //read into an array of char arrays (so we can coppy data easily)
    // increment sent so it moves off sent start delim
    thisSent++;
    for(i =0; i<NMEA_MAX_DATA_ARRAY_SIZE; i++){
        nextSent = strchr(thisSent, NMEA_SENTENCE_DATA_DELIM);
        if (nextSent==NULL){
            break;
        }
        memcpy(outputSent[i].data(),thisSent,nextSent-thisSent);
        *nextSent = '\0'; //terminate the string
        thisSent = nextSent+1;
    };
    memcpy(outputSent[i].data(),thisSent,strlen(thisSent));

   return(i--);//return the number of bytes written
};

void NEO6M::hasNmeaSentance(parsedNmeaSent& parsedSent){
gpgsaFields thisMeasurment;
    if(strcmp(parsedSent[0].data(),"GPGGA")){
        //lattitude
       lastCompleteSample.latt_deg = decChar2Float(parsedSent[thisMeasurment.lat.idx].data());

    }
    else {

        }
};


int NEO6M::configurePort(int fd, int baud){
// adapted from:
// https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
// largely boilerplate
      struct termios tty;
      if (tcgetattr (fd, &tty) != 0){
                perror("Failed to return Uart Port attributes");
                exit(1);//
        }

        if (cfsetospeed(&tty, baud) !=0){
                perror("Failed to set Uart port Send BaudRate");
                exit(2);//
        };

        if (cfsetispeed(&tty, baud) !=0){
                perror("Failed to set Uart port Recieve BaudRate");
                exit(3);//
        };
        /* control modes */
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        tty.c_cflag &= ~PARENB; // Clear the parity bit
        tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
        // tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
        // tty.c_cflag |= CSTOPB;  // Set stop field, two stop bits used in communication

        //Input Modes
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        // Special handling of characters such as carriage return etc
        tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|ICRNL);//IGNCR ommited // disable break processing
        // cannonical read flag return on a linebreak (set to zero for non canon)
        // the echo is to do with terminals since you need to see the chars you type
        tty.c_lflag |= ICANON;                // BF local modes - no signaling chars, no echo,
        // Output modes
        tty.c_oflag = 0;                // BF no remapping, no delays
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
        tty.c_cc[VTIME] = 0.5*10;            // 0.5 seconds read timeout (deciseconds)

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); //BF input modes - shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= 0; //parity bit
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0){
                perror("Failed to Set Uart Port attributes");
                exit(4);//
        }
        return 0;//sucess
};

int NEO6M::testChecksum(char* sentance){
// largely adapted from:
  // https://github.com/craigpeacock/NMEA-GPS/blob/23f15158f124cc2b65c0468d5c690be23a449a77/gps.c#L109
	char *checksum_str;
	int returned_checksum;
	int calculated_checksum = 0;

	// Checksum is postcede by *
	checksum_str = strchr(sentance, NMEA_SENTENCE_CHECKSUM_DELIM);
	if (checksum_str != NULL){
		// Remove checksum from sentance
		// *checksum_str = '\0';

		// loop through remaining sentance to Calculate checksum, starting after $ (i = 1)
  		int lenSent = checksum_str-sentance;
		for (int i = 1; i < lenSent ; i++) {
			calculated_checksum = calculated_checksum ^ sentance[i];
		}

        //returned checksum is apparently in hex characters
		returned_checksum = hexChar2Int((char *)checksum_str+1);
		if (returned_checksum == calculated_checksum) {
			//Checksum is fine
			return(0);
		}
	} else {
#ifdef DEBUG
		fprintf(stderr,"Error: Checksum missing or NULL NMEA message\r\n");
#endif
		return(-1);
	}
	return(-1);
};

int NEO6M::hexChar2Int(char* checksumChar){
    // TODO write a test for this method should be fairly easy!!
    // convert the msb and lsb into ints and then scale msb by a factor of 16
    int msb = hexChar2IntLUT[(*checksumChar)];
    int lsb = hexChar2IntLUT[*(checksumChar+1)];
    if (msb<0){
        fprintf(stderr,"Checksum MSB char %c is not a valid hex char",msb);
        return(-1);
    };
    if (msb<0){
        fprintf(stderr,"Checksum LSB char %c is not a valid hex char",lsb);
        return(-2);
    };
    int checksum = msb*16+lsb;
    return checksum;
};

double NEO6M::decChar2Float(char* thisCharFloat){
    unsigned long int intOut=0;
    double out =0;
    int decimalIdx= -1;
    int thisInt;
    unsigned long order = (unsigned long)pow(10,NMEA_MAX_DATA_FIELD_SIZE-1);
    int i=0;
    while (*(thisCharFloat+i)!='\0'){
        thisInt = decChar2IntLUT[*(thisCharFloat + i)];
        if (thisInt<0){
            if (*(thisCharFloat+i)=='.'){
            decimalIdx = i;
            i++;
            }
            else {
                fprintf(stderr,"Non numeric character detected: \"%c\"\n", *(thisCharFloat+i));
            }
        }
        else {
          order /= 10; // decrease the order by a factor of 10
          intOut += order * decChar2IntLUT[*(thisCharFloat + i)];
          i++;
        }
    }

    if(decimalIdx>0){ // compensate for the fact that we have a floating point
        out =(double)intOut / pow(10,NMEA_MAX_DATA_FIELD_SIZE-1-decimalIdx);
    }
    else {
        out =(double)intOut /(double)order;
    }
    return out;
};

int NEO6M::decChar2Int(char* charInt){
    long long int out=0; //make sure we get no overflows!
    unsigned long order = (unsigned long)pow(10,NMEA_MAX_DATA_FIELD_SIZE-1);
    int thisInt = 0;
    while (*charInt!='\0'){
        thisInt = decChar2IntLUT[*charInt];
        if (thisInt==-1){
            fprintf(stderr,"Invalid Character Detected, this:\"%c\" Can't be an int!\n",*charInt);
            return(-1);
        }
        order/=10;//decrease the order by a factor of 10
        out += order*thisInt;
        charInt++;
    }
    out/=order;
    return out;
};
