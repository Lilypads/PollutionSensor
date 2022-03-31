#include "try1.h"

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

int out = i2cWriteDevice(handle,tmp,4);

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

int out = i2cWriteDevice(handle,pnt,2);
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
int dataREAD = i2cReadDevice(handle,tmp,3);

// i think "dataREAD" is just a flag that indicates the success of the read opperation
if (dataREAD < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not read from %02x. i2cReadDevice() returned %i\n",settings.address,dataREAD);

#endif
                throw could_not_open_i2c;
}

//tryyyyyy
fprintf(stderr,"Device Data: \n");
for (uint i=0; i < sizeof(tmp)+1; i++) {
    fprintf(stderr,"  %.*u \n",(int)sizeof(tmp[i]),tmp[i]);
}
fprintf(stderr,"");
i2cClose(handle);

}
