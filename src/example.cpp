/*#include "sqr.h"
#include "sqr.cpp"
#include "iostream"
#include "sps30.h"
//#include "sps30lib.h"

int main()
{
std::cout << "Hello World!" << sqr(2) << std::endl;
return 0;
}
    */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "try1.h"

#define DEBUG

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
int main()
{

SPS30 mySPS;


SPS30settings s;

mySPS.start(s);
sleep(4);

mySPS.readVersion();
sleep(1);

mySPS.readSerialNumber();
fprintf(stderr, "Serial Number: %.*s\n",SN_LEN_WO_SRC,mySPS.serialNumber);
sleep(1);

int dReadyFlag =  mySPS.readDRDYFlag();
fprintf(stderr, "Data ready flag: %d\n", dReadyFlag);
sleep(1);

SPS30measurement measurmentOut = mySPS.readMeasurement();
fprintf(stderr,"PM1.0: %f\n",measurmentOut.MassConcPM1_0);
fprintf(stderr,"Typical Particle size: %f\n", measurmentOut.TypicalParcSize);
sleep(1);


fprintf(stderr,"Press any key to stop.\n");
getchar();


mySPS.stop();
 return 0;

}
