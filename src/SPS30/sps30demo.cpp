#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "libsps30.h"

#define DEBUG


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

mySPS.setSettings(s);
mySPS.readSerialNumber();
fprintf(stderr,"Serial Number: %s\n",mySPS.serialNumber);
mySPS.readVersion();

fprintf(stderr,"Press any key to stop.\n");
mySPS.startMeasurement();


getchar();


mySPS.stop();

 return 0;

}
