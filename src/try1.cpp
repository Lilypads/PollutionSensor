#include "try1.h"

void SPS30::start(SPS30settings settings){

int handle = i2cOpen(settings.i2c_bus, settings.address,0);
        if (handle < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c;
        }
char tmp[4];  //pointer & data to write
	tmp[0] = (char)(( START_MEASUREMENT_B1 & 0xff00) >> 8);
	tmp[1] = (char)(START_MEASUREMENT_B2 & 0x00ff);
	tmp[2] = (char)(( BIG_ENDIAN_IEEE754_FLOAT_TYPE & 0xff00) >> 8);
	tmp[3] = (char)(DUMMY & 0x00ff);

int i2cWriteDevice(handle,tmp,4)

i2cClose(handle)
}

void SPS30::stop(SPS30settings settings){

int handle = i2cOpen(settings.i2c_bus, settings.address,0);
        if (handle < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c;

char pnt[2];  //pointer
	pnt[0] = (char)(( STOP_MEASUREMENT_B1 & 0xff00) >> 8);
	pnt[1] = (char)(STOP_MEASUREMENT_B2 & 0x00ff);

int i2cWriteDevice(handle,pnt,2)
i2cClose(handle)

}

void SPS30::readVersion(SPS30settings settings){

int handle = i2cOpen(settings.i2c_bus, settings.address,0);
        if (handle < 0) {
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c;

char pnt[2];  //pointer
	pnt[0] = (char)(( READ_VERSION_B1 & 0xff00) >> 8);
	pnt[1] = (char)(READ_VERSION_B2 & 0x00ff);

char tmp[3];  //buffer for read data

int i2cWriteDevice(handle,pnt,2)
int i2cREADDevice(handle,tmp,3)

i2cClose(handle)

}