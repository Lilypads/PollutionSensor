#include <iostream>
#include <string>
#include <stdio.h>
#include "storageHandler.h"
#include "libsps30.h"
#include "neo6m.h"
#include "stateMachine.h"
#include "inputHandler.h"
#include "displayHandler.h"
#include "menuOptions.h"
#include "menuData.h"
#include "menuHandler.h"

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


//userInterface
class bindStartStopMeasurement
{
public:

    void startMeasurement(std::mutex& m)
    {
         mySPS30.startMeasurement();
         s.startAcquisition();

    };

    void stopMeasurement(std::mutex& m)
    {
         mySPS30.stop();
         s.stopAcquisition();

    };
};

MenuHandler handler;
bindStartStopMeasurement mybindStartStopMeasurement;


int main()
{
    //initialise
    myStorageHandler.identificationNumber = "1";
    myStorageHandler.createFiles();
    myGPS.startMeasurement();

    handler.Init();
    handler.menu.measureMenu.buttons[0].function = std::bind<void(bindStartStopMeasurement::*)(std::mutex&), bindStartStopMeasurement*>(&bindStartStopMeasurement::startMeasurement, &mybindStartStopMeasurement, std::placeholders::_1);
    handler.menu.measureMenu.buttons[1].function = std::bind<void(bindStartStopMeasurement::*)(std::mutex&), bindStartStopMeasurement*>(&bindStartStopMeasurement::stopMeasurement, &mybindStartStopMeasurement, std::placeholders::_1);
    handler.Start();

    //clean up
    s.shutdown();
    myGPS.stopMeasurement();
    myStorageHandler.closeFiles();
    

    return 0;
};