#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "storageHandler.h"

int main(){

    storageHandler myStorageHandler;
    //set identification number that will go into file name later
    myStorageHandler.identificationNumber = "80";
    myStorageHandler.createFiles();

    //sample GPS data
    neo6mMeasurment sampleGPSdata;
    sampleGPSdata.latt_deg = 11;
    sampleGPSdata.long_deg = 12;
    sampleGPSdata.hdop = 13;
    sampleGPSdata.alt_m = 14;
    sampleGPSdata.utc = 15;
    sampleGPSdata.tLastUpdate = 16;
    sampleGPSdata.fixQuality = 17;

    //add buffer GPS measurememt
    myStorageHandler.bufferMeasurement = sampleGPSdata;

    //sample SPS data
    SPS30measurement sampleSPSdata;
    sampleSPSdata.MassConcPM1_0 = 1;
    sampleSPSdata.MassConcPM2_5 = 2;
    sampleSPSdata.MassConcPM4_0 = 3;
    sampleSPSdata.MassConcPM10_0 = 4;
    sampleSPSdata.NumConcPM0_5 = 5;
    sampleSPSdata.NumConcPM1_0 = 6;
    sampleSPSdata.NumConcPM2_5 = 7;
    sampleSPSdata.NumConcPM4_0 = 8;
    sampleSPSdata.NumConcPM10_0 = 9;
    sampleSPSdata.TypicalParcSize = 10;

    // save SPS30 measurement along with last GPS measurement to file
    myStorageHandler.addMeasurement(sampleSPSdata);
    myStorageHandler.addMeasurement(sampleSPSdata);
    myStorageHandler.closeFiles();


return 0;
}
