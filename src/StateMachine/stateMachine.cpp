#include "stateMachine.h"

// constructor
stateMachine::stateMachine(){
};

//destructor
stateMachine::~stateMachine(){
    shutdown();
};

void stateMachine::initiate(){

//storage handler
    storageHandler myStorageHandler;
    myStorageHandler.identificationNumber = "1";
    myStorageHandler.createFiles();

//sps30
    class SPS30Acquiring: public SPS30 {
    virtual void hasMeasurmentCB(SPS30measurement thisMeasurement){
        
        myStorageHandler.addMeasurement(thisMeasurement);
    }
};

    SPS30Acquiring mySPS30;

//gps
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

    neo6m myGPS();

    myGPS.startMeasurement();


};


    
};

void stateMachine::shutdown(){
    myStorageHandler.closeFiles();
    ~mySPS30();
    ~myGPS();
    ~myStorageHandler();

};

void stateMachine::startAcquisition(){

    mySPS30.startMeasurement();

    
};

void stateMachine::stopAcquisition(){
    mySPS30.stop();
    myGPS.stopMeasurement();

};

void stateMachine::startNewTripLog(string identificationNumber){
    myStorageHandler.closeFiles();
    myStorageHandler.identificationNumber = identificationNumber;
    myStorageHandler.createFiles();


};
