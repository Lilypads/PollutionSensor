#include "stateMachine.h"

// constructor
stateMachine::stateMachine(){
};

//destructor
stateMachine::~stateMachine(){
    
};

void stateMachine::fixedAcquired(){
    if(currentState==states.AcquiringGPSFix){
    currentState = states.Active;
    }
};
void stateMachine::fixedLost(){
    if(currentState==states.Active){
    currentState = states.AcquiringGPSFix;
    }
};
void stateMachine::startAcquisition(){
    if(currentState==states.Active){
    currentState = states.AcquiringData;
    }
};
void stateMachine::stopAcquisition(){
    if(currentState==states.AcquiringData){
    currentState = states.Active;
    }
};
void stateMachine::shutdown(){
    if(currentState==states.Active||currentState==states.AcquiringGPSFix){
    currentState = states.CleanUp;
    }
};