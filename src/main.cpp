#include <iostream>
#include <pigpio.h>
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

#define GPIO_INCREMENT_PIN 19
#define GPIO_DECREMENT_PIN 6
#define GPIO_SELECT_PIN 26
#define DEBOUNCE_TIMEOUT_US 300000 //20ms debounce timeout

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

void gpioIncrementISR(int gpio,int level, uint32_t tick){
    // fprintf(stderr,"Inclrement detected");
    // handler.menu.CursorUp(0);
    handler.m.lock();
    handler.inputMonitor.inputs.push(1);
    handler.m.unlock();
};

void gpioDecrementISR(int gpio,int level, uint32_t tick){
    // fprintf(stderr,"Inclrement detected");
    // handler.menu.CursorDown(0);
    handler.m.lock();
    handler.inputMonitor.inputs.push(2);
    handler.m.unlock();
};
void gpioSelectISR(int gpio,int level, uint32_t tick){
    // fprintf(stderr,"Inclrement detected");
    // handler.menu.Select(0);
    handler.m.lock();
    handler.inputMonitor.inputs.push(0);
    handler.m.unlock();
};
int main()
{

    //initialise
    DataWriter writer;
    std::string name = "tripLog_1.csv";
    FILE *fileCheck = fopen(name.c_str(), "r");
    int checkNum = 1;

    //bind isr
    gpioInitialise();
    gpioSetMode(GPIO_INCREMENT_PIN,       PI_INPUT);
    gpioSetMode(GPIO_DECREMENT_PIN,       PI_INPUT);
    gpioSetMode(GPIO_SELECT_PIN,          PI_INPUT);
    gpioSetPullUpDown(GPIO_INCREMENT_PIN, PI_PUD_UP);
    gpioSetPullUpDown(GPIO_DECREMENT_PIN, PI_PUD_UP);
    gpioSetPullUpDown(GPIO_SELECT_PIN,    PI_PUD_UP);
    gpioSetISRFunc(GPIO_INCREMENT_PIN, FALLING_EDGE, 0, gpioIncrementISR);
    gpioSetISRFunc(GPIO_DECREMENT_PIN, FALLING_EDGE, 0, gpioDecrementISR);
    gpioSetISRFunc(GPIO_SELECT_PIN,    FALLING_EDGE, 0, gpioSelectISR);
    gpioGlitchFilter(GPIO_INCREMENT_PIN, DEBOUNCE_TIMEOUT_US);
    gpioGlitchFilter(GPIO_DECREMENT_PIN, DEBOUNCE_TIMEOUT_US);
    gpioGlitchFilter(GPIO_SELECT_PIN   , DEBOUNCE_TIMEOUT_US);

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
