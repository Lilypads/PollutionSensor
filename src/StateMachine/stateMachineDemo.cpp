#include "stateMachine.h"


int main()
{
    stateMachine myMachine;
    myMachine.initiate();
    //Acquiring GPS fix state
    while(!myMachine.gpsFixedFlag)
    {
        //infinite loop
    };
    //ActiveState
    myMachine.startAcquisition();

    fprintf(stderr,"Type any char to stop acquisition");
    getchar();

    myMachine.stopAcquisition();

    myMachine.startNewTripLog("2");

    myMachine.startAcquisition();

    fprintf(stderr,"Type any char to stop acquisition");
    getchar();

    myMachine.stopAcquisition();

    myMachine.shutdown();

  return 0;
}