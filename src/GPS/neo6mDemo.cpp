#include <neo6m.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
// #define DEBUG
class neo6m: public NEO6M{
    using NEO6M::NEO6M; //inherit constructor
    virtual void hasMeasurementCB(neo6mMeasurment m){
    printSample(m);
    }
};

int main(){
    neo6mSettings s;
    neo6m gps(s);

    fprintf(stderr,"Starting measurement... Press any key to stop.\n");
    gps.startMeasurement();

    getchar();
    gps.stopMeasurement();
    return 0;
};
