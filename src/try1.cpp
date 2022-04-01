#include "try1.h"

float bytesToFloat(char b3, char b2, char b1, char b0){
	float f;
	char b[] = {b3, b2, b1, b0};
	memcpy(&f, &b, sizeof(f));
	return f;
};

void SPS30::start(SPS30settings settings){

if(settings.initPIGPIO){
         int cfg = gpioCfgGetInternals();
         cfg |= PI_CFG_NOSIGHANDLER;  // (1<<10)
         gpioCfgSetInternals(cfg);
         int status = gpioInitialise();
         if(status<0){
               char msg[] = "cannot init pigpio.";
            }
    }

int handle = i2cOpen(settings.i2c_bus, settings.address,0);

        fprintf(stderr,"I2C Buss: %u\n",settings.i2c_bus);
        fprintf(stderr,"I2C Address: %x\n",settings.address);
        fprintf(stderr,"I2C Handle: %i\n",handle);

        if (handle < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c;
        }
char tmp[4];  //pointer & data to write
	tmp[0] = (char)(( START_MEASUREMENT & 0xff00) >> 8);
	tmp[1] = (char)(START_MEASUREMENT & 0x00ff);
	tmp[2] = (char)(BIG_ENDIAN_IEEE754_FLOAT_TYPE);
	tmp[3] = (char)(DUMMY);

int checkERR = i2cWriteDevice(handle,tmp,4);

        if (checkERR != 0) {
#ifdef DEBUG
                fprintf(stderr,"Cannot start measurement.\n");
#endif
        }
i2cClose(handle);
}

void SPS30::stop(){

int handle = i2cOpen(settings.i2c_bus, settings.address,0);
        if (handle < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c;
        }
char pnt[2];  //pointer
	pnt[0] = (char)(( STOP_MEASUREMENT & 0xff00) >> 8);
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

int SPS30::readDRDYFlag(){

int handle = i2cOpen(settings.i2c_bus, settings.address,0);
        if (handle < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c;
        }
char pnt[2];  //pointer
	pnt[0] = (char)(( READ_DRDY_FLAG & 0xff00) >> 8);
	pnt[1] = (char)(READ_DRDY_FLAG & 0x00ff);

char tmp[3];  //buffer for read data

int out = i2cWriteDevice(handle,pnt,2);
int checkERR = i2cReadDevice(handle,tmp,3);

if (checkERR < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not read from %02x. i2cReadDevice() returned %i\n",settings.address,checkERR);

#endif
                throw could_not_open_i2c;
}

#ifdef DEBUG // comment this out after
fprintf(stderr,"DRDY Flag: %u\n",(int)tmp[1]);
#endif

i2cClose(handle);

return tmp[1];

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
int checkERR = i2cReadDevice(handle,tmp,3);

if (checkERR < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not read from %02x. i2cReadDevice() returned %i\n",settings.address,checkERR);

#endif
                throw could_not_open_i2c;
}

i2cClose(handle);

//get tmp into float >> put in struct
//measurements = method(tmp)


/*
 we honestly are sorry.
*/

int i = SPS30dataOutputIdx::idxMassConcPM1_0;
measurements.MassConcPM1_0 = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i = SPS30dataOutputIdx::idxMassConcPM2_5;
measurements.MassConcPM2_5 = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i = SPS30dataOutputIdx::idxMassConcPM4_0;
measurements.MassConcPM4_0 = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i = SPS30dataOutputIdx::idxMassConcPM10_0;
measurements.MassConcPM10_0 = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);

i = SPS30dataOutputIdx::idxNumConcPM0_5;
measurements.NumConcPM0_5 = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i = SPS30dataOutputIdx::idxNumConcPM1_0;
measurements.NumConcPM1_0 = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i = SPS30dataOutputIdx::idxNumConcPM2_5;
measurements.NumConcPM2_5 = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i = SPS30dataOutputIdx::idxNumConcPM4_0;
measurements.NumConcPM4_0 = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);
i = SPS30dataOutputIdx::idxNumConcPM10_0;
measurements.NumConcPM10_0 = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);

i = SPS30dataOutputIdx::idxTypicalParcSize;
measurements.TypicalParcSize = bytesToFloat(tmp[i],tmp[i+1],tmp[i+3],tmp[i+4]);

return measurements;

}
