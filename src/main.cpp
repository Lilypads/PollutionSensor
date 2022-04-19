#include <iostream>
#include <string>
#include <stdio.h>
#include <storageHandler.h>
#include <libsps30.h>
#include <neo6m.h>
#include <stateMachine.h>
#include <inputHandler.h>
#include <displayHandler.h>
#include <menuOptions.h>
#include <menuData.h>
#include <menuHandler.h>

std::string identificationNumber;
stateMachine s;
storageHandler myStorageHandler;

SPS30measurement lastSPSMeasurement;
neo6mMeasurment lastGPSMeasurement;

class DataWriter
{
public:
    MenuHandler* menuHandler;

    void UpdateReading()
    {
        menuHandler->ForceDisplayUpdate();
    };
};

//sps30 inheritance class
    class SPS30Acquiring: public SPS30 {
    virtual void hasMeasurmentCB(SPS30measurement thisMeasurement){
        
        //DataWriter writer;
        
        //writer.UpdateReading();

        lastSPSMeasurement = thisMeasurement;

        if (s.currentState == AcquiringData){
        myStorageHandler.addMeasurement(thisMeasurement);
        }
    }
};
//gps inheritance class
    class neo6m: public NEO6M{
    using NEO6M::NEO6M; //inherit constructor
    virtual void hasMeasurementCB(neo6mMeasurment m){

        lastGPSMeasurement = m;

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

    void startMeasurement()
    {
         s.startAcquisition();
    };

    void stopMeasurement()
    {
         s.stopAcquisition();
    };
};

MenuHandler handler;
bindStartStopMeasurement mybindStartStopMeasurement;

int main()
{

    //initialise
    DataWriter writer;
    std::string name = "tripLog_1.csv";
    FILE *fileCheck = fopen(name.c_str(), "r");
    int checkNum = 1;

    while(fileCheck)
    {
        fclose(fileCheck);
        checkNum++;
        name = "tripLog_" + std::to_string(checkNum) + ".csv";
        fileCheck = fopen(name.c_str(), "r");
    };

    myStorageHandler.identificationNumber = std::to_string(checkNum);

    myStorageHandler.createFiles();

    //Initialise user interface
    handler.Init();

    writer.menuHandler = &handler;

    //Set button functions that are external to UI system
    handler.menu.measureMenu.buttons[0].function = std::bind<void(bindStartStopMeasurement::*)(), bindStartStopMeasurement*>(&bindStartStopMeasurement::startMeasurement, &mybindStartStopMeasurement);
    handler.menu.measureMenu.buttons[1].function = std::bind<void(bindStartStopMeasurement::*)(), bindStartStopMeasurement*>(&bindStartStopMeasurement::stopMeasurement, &mybindStartStopMeasurement);
    
    handler.display.displayPollution = &lastSPSMeasurement;
    handler.display.displayGPS = &lastGPSMeasurement;

    myGPS.startMeasurement();
    mySPS30.startMeasurement(); //This causes an error, I'm unsure why or how to fix it

    //Start user interface
    handler.Start();

    //clean up
    s.shutdown();
    mySPS30.stop();
    myGPS.stopMeasurement();
    myStorageHandler.closeFiles();
    

    return 0;
};
