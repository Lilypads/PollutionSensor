#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "libsps30.h"

#define DEBUG

/**
 * calculate checksum locally
 **/
uint8_t CalcCrc_Local(uint8_t data[2]) {
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

class SPS30Printer: public SPS30 {
    virtual void hasMeasurmentCB(SPS30measurement thisMeasurement){
        fprintf(stderr,"PM1.0: %f\n",thisMeasurement.MassConcPM1_0);
	fprintf(stderr,"Typical Particle size: %f\n", thisMeasurement.TypicalParcSize);
    }
};

int main()
{

SPS30Printer mySPS;

SPS30settings s;
s.autoStartThread = false;

mySPS.startMeasurement(s);
mySPS.readSerialNumber();
mySPS.readVersion();
fprintf(stderr,"Press any key to stop.\n");

getchar();


mySPS.stop();

 return 0;

}
