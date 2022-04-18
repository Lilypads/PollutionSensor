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
    }
};
void stateMachine::fixedLost(){
    if(currentState==Active){
    currentState = AcquiringGPSFix;
    }
};
void stateMachine::startAcquisition(){
    if(currentState==Active){
    currentState = AcquiringData;
    }
};
void stateMachine::stopAcquisition(){
    if(currentState==AcquiringData){
    currentState = Active;
    }
};
void stateMachine::shutdown(){
    if(currentState==Active||currentState==AcquiringGPSFix){
    currentState = CleanUp;
    }
};