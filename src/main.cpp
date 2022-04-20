#include <iostream>
#include <pigpio.h>
#include <string>
#include <time.h>
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
#include <gitWrapper.h>

#define GPIO_INCREMENT_PIN 19
#define GPIO_DECREMENT_PIN 6
#define GPIO_SELECT_PIN 26
#define DEBOUNCE_TIMEOUT_US 300000 //20ms debounce timeout

#define LOGDATA_PATH "/var/lib/polsense/triplogs/"

std::string identificationNumber;
stateMachine s;
storageHandler myStorageHandler;

SPS30measurement lastSPSMeasurement;
neo6mMeasurment lastGPSMeasurement;

MenuHandler handler;

//sps30 inheritance class
    class SPS30Acquiring: public SPS30 {
    virtual void hasMeasurmentCB(SPS30measurement thisMeasurement){
        
        handler.ForceDisplayUpdate();

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
        //Create a file to write to
        myStorageHandler.createFiles();

        //Start writing to the file
         s.startAcquisition();
    };

    void stopMeasurement()
    {

        //Stop writing to a file
         s.stopAcquisition();

         //Close the file
         myStorageHandler.closeFiles();
    };
};

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
    int checkNum = 1;

    //Set up github wrapper
    gitwrapperSettings gitS;
    strcpy(gitS.path, LOGDATA_PATH);
    strcpy(gitS.remote, "git@github.com:BodeanTheZealous/PollutionSensorData.git");
    strcpy(gitS.sshKeysFileAbs, "~/Repos/PollutionSensor/src/GitWrapper/pollSenseKey");
    GITWRAPPER gw(gitS);

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

    //Initialise user interface
    handler.Init();

    //Set button functions that are external to UI system
    handler.menu.measureMenu.buttons[0].function = std::bind<void(bindStartStopMeasurement::*)(), bindStartStopMeasurement*>(&bindStartStopMeasurement::startMeasurement, &mybindStartStopMeasurement);
    handler.menu.measureMenu.buttons[1].function = std::bind<void(bindStartStopMeasurement::*)(), bindStartStopMeasurement*>(&bindStartStopMeasurement::stopMeasurement, &mybindStartStopMeasurement);
    
    //Point the display at the relevant external data containers
    handler.display.displayPollution = &lastSPSMeasurement;
    handler.display.displayGPS = &lastGPSMeasurement;
    handler.display.stateName = &s.stateName;
    handler.display.fileName = &myStorageHandler.fileName;

    //Start up the GPS and SPS30 sensor
    myGPS.startMeasurement();
    mySPS30.startMeasurement();

    //Start user interface
    handler.Start();

    //clean up
    s.shutdown();
    mySPS30.stop();
    myGPS.stopMeasurement();

    return 0;
};
