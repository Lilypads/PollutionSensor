#include <iostream>
#include <string>
#include <stdio.h>
#include "storageHandler.h"
#include "libsps30.h"
#include "neo6m.h"

std::string identificationNumber;
bool gpsFixedFlag;
storageHandler myStorageHandler;

//sps30 inheritance class
    class SPS30Acquiring: public SPS30 {
    virtual void hasMeasurmentCB(SPS30measurement thisMeasurement){
        
        myStorageHandler.addMeasurement(thisMeasurement);
    }
};
//gps inheritance class
    class neo6m: public NEO6M{
    using NEO6M::NEO6M; //inherit constructor
    virtual void hasMeasurementCB(neo6mMeasurment m){
        myStorageHandler.bufferMeasurement = m;
        if(m.fixQuality==1) //fixed
        {
            gpsFixedFlag = 1; //isFixed? = true
        }
        else
        {
            gpsFixedFlag = 0; //isFixed? = false;
        }
   }
    };

//sps30
SPS30Acquiring mySPS30;
//gps
neo6m myGPS;


void stateMachine_initiate(){

    //storage handler
    myStorageHandler.identificationNumber = "1";
    myStorageHandler.createFiles();
    myGPS.startMeasurement();
}

void stateMachine_shutdown(){
    myGPS.stopMeasurement();
    myStorageHandler.closeFiles();

}

void stateMachine_startAcquisition(){
    mySPS30.startMeasurement();
}

void stateMachine_stopAcquisition(){
    mySPS30.stop();

}

void stateMachine_startNewTripLog(std::string identificationNumber){
    myStorageHandler.closeFiles();
    myStorageHandler.identificationNumber = identificationNumber;
    myStorageHandler.createFiles();


}

int main()
{
    stateMachine_initiate();
    //Acquiring GPS fix state
    while(!gpsFixedFlag)
    {
        //infinite loop
    };
    //ActiveState
    stateMachine_startAcquisition();

    fprintf(stderr,"Type any char to stop acquisition");
    getchar();

    stateMachine_stopAcquisition();

    stateMachine_startNewTripLog("2");

    stateMachine_startAcquisition();

    fprintf(stderr,"Type any char to stop acquisition");
    getchar();

    stateMachine_stopAcquisition();

    stateMachine_shutdown();

  return 0;
}