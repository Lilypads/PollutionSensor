#include "stateMachine.h"

// constructor
stateMachine::stateMachine(){
};

//destructor
stateMachine::~stateMachine(){
    
};

void stateMachine::fixedAcquired(){
    if(currentState==AcquiringGPSFix){
    currentState = Active;
    stateName = "Active";
    }
};
void stateMachine::fixedLost(){
    if(currentState==Active){
    currentState = AcquiringGPSFix;
    stateName = "Acquiring GPS Fix";
    }
};
void stateMachine::startAcquisition(){
    if(currentState==Active){
    currentState = AcquiringData;
    stateName = "Acquiring Data";
    }
};
void stateMachine::stopAcquisition(){
    if(currentState==AcquiringData){
    currentState = Active;
    stateName = "Active";
    }
};
void stateMachine::shutdown(){
    if(currentState==Active||currentState==AcquiringGPSFix){
    currentState = CleanUp;
    stateName = "Cleanup";
    }
};