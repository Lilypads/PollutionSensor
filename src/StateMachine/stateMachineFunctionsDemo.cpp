#include <iostream>
#include <string>
#include <stdio.h>
#include "storageHandler.h"
#include "libsps30.h"
#include "neo6m.h"
#include "stateMachine.h"

std::string identificationNumber;
stateMachine s;
storageHandler myStorageHandler;

//sps30 inheritance class
    class SPS30Acquiring: public SPS30 {
    virtual void hasMeasurmentCB(SPS30measurement thisMeasurement){
        
        if (s.currentState == AcquiringData){
        myStorageHandler.addMeasurement(thisMeasurement);
        }
    }
};
//gps inheritance class
    class neo6m: public NEO6M{
    using NEO6M::NEO6M; //inherit constructor
    virtual void hasMeasurementCB(neo6mMeasurment m){
        myStorageHandler.bufferMeasurement = m;
        if(m.fixQuality==gpsFix||m.fixQuality==dgpsFix)
        {
            s.fixedAcquired(); 
        }
        else
        {
            s.fixedLost(); 
        }
   }
    };

//sps30
SPS30Acquiring mySPS30;
//gps
neo6m myGPS;

int main()
{
    //initialise
    myStorageHandler.identificationNumber = "1";
    myStorageHandler.createFiles();
    myGPS.startMeasurement();

    //start logging
    mySPS30.startMeasurement();

    fprintf(stderr,"Type any char to stop acquisition");
    getchar();

    //stop logging
    mySPS30.stop();

    //start new log file session
    myStorageHandler.closeFiles();
    myStorageHandler.identificationNumber = "2";
    myStorageHandler.createFiles();;

    //second logging
    mySPS30.startMeasurement();

    fprintf(stderr,"Type any char to stop acquisition");
    getchar();

    mySPS30.stop();

    //clean up
    myGPS.stopMeasurement();
    myStorageHandler.closeFiles();

  return 0;
}