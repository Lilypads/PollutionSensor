#include "libsps30.h"

uint8_t CalcCrc(uint8_t data[2]) {
    uint8_t crc = 0xFF;

    for(int i = 0; i < 2; i++) {
        crc ^= data[i];
        for(uint8_t bit = 8; bit > 0; --bit) {
            if(crc & 0x80) {
                crc = (crc << 1) ^ 0x31u;
            } else {
                crc = (crc << 1);
            }
        }
    }
    return crc;
};

float bytesToFloat(char b0, char b1, char b2, char b3){
	float f;
	char b[] = {b3, b2, b1, b0};
	memcpy(&f, &b, sizeof(f));
	return f;
};

// constructor
SPS30::SPS30(SPS30settings userSettings){
    settings = userSettings;

if(settings.initPIGPIO){
         int cfg = gpioCfgGetInternals();
         cfg |= PI_CFG_NOSIGHANDLER;  // (1<<10)
         gpioCfgSetInternals(cfg);
         int status = gpioInitialise();
         if(status<0){
               char msg[] = "cannot init pigpio.";
            }
    }

};

void SPS30::setSettings(SPS30settings newSettings){
settings = newSettings;
};

//destructor
SPS30::~SPS30(){
    stop();
};

void SPS30::startMeasurement(){
int handle = i2cOpen(settings.i2c_bus, settings.address,0);

#ifdef DEBUG
        fprintf(stderr,"I2C Buss: %u\n",settings.i2c_bus);
        fprintf(stderr,"I2C Address: %x\n",settings.address);
        fprintf(stderr,"I2C Handle: %i\n",handle);
#endif
        if (handle < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c;
        }


char tmp[5];  //pointer & data to write

    // command byte
	tmp[0] = (char)((START_MEASUREMENT & 0xff00) >> 8);
	tmp[1] = (char)(START_MEASUREMENT & 0x00ff);
    // payload
	tmp[2] = (char)(BIG_ENDIAN_IEEE754_FLOAT_TYPE);
	tmp[3] = (char)(DUMMY);

    uint8_t tempData[2];
   tempData[0] = tmp[2];
   tempData[1] = tmp[3];

uint8_t checksum = CalcCrc(tempData);

    //checksum
    tmp[4] = (char)(checksum);

#ifdef DEBUG
   fprintf(stderr, "StartMesurement Command:\n");
   for (int i = 0; i < sizeof(tmp); i++) {
    fprintf(stderr, "%u,",(uint)tmp[i]);
   }
    fprintf(stderr, "\n");
#endif

usleep(50000);

int checkERR = i2cWriteDevice(handle,tmp,5);

        if (checkERR != 0) {
#ifdef DEBUG
                fprintf(stderr,"Cannot start measurement.\n");
#endif
        }
i2cClose(handle);


// start up daq thread

if (nullptr != daqThread) {
    // already running
    return;
}

if(settings.autoStartThread){  
    //start the polling thread
    daqThread = new std::thread(execPollingThread,this);
}


};

void SPS30::pollDRDYFlag(){
isPollingDRDY = true;
    while (isPollingDRDY) {
        if(readDRDYFlag()){
            hasMeasurmentCB(readMeasurement());
        }
        usleep(DRDY_POLLINGPERIOD_US);
    }
};

void SPS30::stop(){
// stop the polling thread
isPollingDRDY = false;


//wait for polling thread to finish
if (nullptr != daqThread) {
    daqThread->join();
    delete daqThread;
    daqThread = nullptr;
}

// move device out of mesurement mode
int handle = i2cOpen(settings.i2c_bus, settings.address,0);
        if (handle < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c;
        }
char pnt[2];  //pointer
	pnt[0] = (char)((STOP_MEASUREMENT & 0xff00) >> 8);
	pnt[1] = (char)(STOP_MEASUREMENT & 0x00ff);

int checkERR = i2cWriteDevice(handle,pnt,2);
        if (checkERR != 0) {
#ifdef DEBUG
                fprintf(stderr,"Cannot stop measurement.\n");
#endif
        }

i2cClose(handle);

}

void SPS30::readVersion(){

int handle = i2cOpen(settings.i2c_bus, settings.address,0);
        if (handle < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c;
        }
char pnt[2];  //pointer
	pnt[0] = (char)(( READ_VERSION & 0xff00) >> 8);
	pnt[1] = (char)(READ_VERSION & 0x00ff);

char tmp[3];  //buffer for read data

int out = i2cWriteDevice(handle,pnt,2);
int checkERR = i2cReadDevice(handle,tmp,3);

// i think "dataREAD" is just a flag that indicates the success of the read opperation >> it is! sorry my bad ;< (Lily)
if (checkERR < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not read from %02x. i2cReadDevice() returned %i\n",settings.address,checkERR);

#endif
                throw could_not_open_i2c;
}

fprintf(stderr,"Device Firmware Version: %u,%u\n",(int)tmp[0],(int)tmp[1]);

i2cClose(handle);

}

int SPS30::readSerialNumber(){

int handle = i2cOpen(settings.i2c_bus, settings.address,0);
        if (handle < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c;
        }

char sendBuff[3];
char retBuff[SN_LEN_W_SRC]="";
sendBuff[0] = (char)((READ_SERIAL_NUMBER>>8) & 0xff);
sendBuff[1] = (char)((READ_SERIAL_NUMBER) & 0xff);

int out = i2cWriteDevice(handle,(char*)sendBuff,2);
usleep(50000);
int checkERR = i2cReadDevice(handle,retBuff,SN_LEN_W_SRC);

if (checkERR < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not read from %02x. i2cReadDevice() returned %i\n",settings.address,checkERR);

#endif
                throw could_not_open_i2c;
}

// loop through all of the returned data and pick out the serial number
for (uint i = 0; i < SN_LEN_WO_SRC/2; i++) {
int thisBuffIdx1= i*3;
int thisBuffIdx2= i*3+1;

int thisSNIdx1= i*2;
int thisSNIdx2= i*2+1;

serialNumber[thisSNIdx1] = retBuff[thisBuffIdx1];
serialNumber[thisSNIdx2] = retBuff[thisBuffIdx2];
}


i2cClose(handle);

return 1;

}


int SPS30::readDRDYFlag(){

int handle = i2cOpen(settings.i2c_bus, settings.address,0);
        if (handle < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c;
        }
char sendBuff[2];  //pointer
	sendBuff[0] = (char)(( READ_DRDY_FLAG & 0xff00) >> 8);
	sendBuff[1] = (char)(READ_DRDY_FLAG & 0x00ff);

char retBuff[3];  //buffer for read data

int out = i2cWriteDevice(handle,sendBuff,2);
usleep(50000); //sleep for 1/2 seconds
int checkERR = i2cReadDevice(handle,retBuff,3);

if (checkERR < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not read from %02x. i2cReadDevice() returned %i\n",settings.address,checkERR);

#endif
                throw could_not_open_i2c;
}

// check crc
uint8_t data2CrcCheck[2];
data2CrcCheck[0] = retBuff[0];
data2CrcCheck[1] = retBuff[1];
uint8_t calculatedCRC = CalcCrc(data2CrcCheck);
bool passCRC = (uint8_t)retBuff[2]==calculatedCRC ;

#ifdef DEBUG
if (!passCRC){
fprintf(stderr,"readDRDTFlag()->Returned Data Failed CRC!\n");
fprintf(stderr,"  ReturnedCheckSum: %u\n",uint8_t(retBuff[3]));
fprintf(stderr,"  CalculatedCheckSum: %u\n",calculatedCRC);
fprintf(stderr,"\n");
}
#endif

//#ifdef DEBUG // comment this out after
//fprintf(stderr,"DRDY Flag: %u\n",(uint8_t)retBuff[1]);
//#endif

i2cClose(handle);


return retBuff[1];

}




SPS30measurement SPS30::readMeasurement(){

SPS30measurement measurements;

int handle = i2cOpen(settings.i2c_bus, settings.address,0);
        if (handle < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c;
        }
char pnt[2];  //pointer
	pnt[0] = (char)(( READ_MEASURED_VALUES & 0xff00) >> 8);
	pnt[1] = (char)(READ_MEASURED_VALUES & 0x00ff);

char tmp[60];  //buffer for read data

int out = i2cWriteDevice(handle,pnt,2);
usleep(50000); //sleep for 1/2 seconds
int checkERR = i2cReadDevice(handle,tmp,sizeof(tmp));

if (checkERR < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not read from %02x. i2cReadDevice() returned %i\n",settings.address,checkERR);

#endif
                throw could_not_open_i2c;
}

i2cClose(handle);

//print raw data
#ifdef DEBUG
int n;
for(n=0;n<60;n++)
fprintf(stderr,"%02X\n",tmp[n]);
#endif

//get tmp into float >> put in struct
//measurements = method(tmp)

/*
 we honestly are sorry.
*/

int i = SPS30dataOutputIdx::idxMassConcPM1_0;
measurements.MassConcPM1_0  = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i = SPS30dataOutputIdx::idxMassConcPM2_5;
measurements.MassConcPM2_5  = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i = SPS30dataOutputIdx::idxMassConcPM4_0;
measurements.MassConcPM4_0  = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i = SPS30dataOutputIdx::idxMassConcPM10_0;
measurements.MassConcPM10_0 = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);

i  = SPS30dataOutputIdx::idxNumConcPM0_5;
measurements.NumConcPM0_5   = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i  = SPS30dataOutputIdx::idxNumConcPM1_0;
measurements.NumConcPM1_0   = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i  = SPS30dataOutputIdx::idxNumConcPM2_5;
measurements.NumConcPM2_5   = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i  = SPS30dataOutputIdx::idxNumConcPM4_0;
measurements.NumConcPM4_0   = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i  = SPS30dataOutputIdx::idxNumConcPM10_0;
measurements.NumConcPM10_0  = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);

i = SPS30dataOutputIdx::idxTypicalParcSize;
measurements.TypicalParcSize = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);


return measurements;
}
