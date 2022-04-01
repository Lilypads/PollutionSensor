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

int main()
{

SPS30 mySPS;


SPS30settings s;

mySPS.start(s);
mySPS.readVersion();

int dReadyFlag =  mySPS.readDRDYFlag();
fprintf(stderr, "Data ready flag: %d", dReadyFlag);

SPS30measurement measurmentOut = mySPS.readMeasurement();
fprintf(stderr,"PM1.0:%f",measurmentOut.MassConcPM1_0);

fprintf(stderr,"Press any key to stop.\n");
getchar();

mySPS.stop();
 return 0;
}
