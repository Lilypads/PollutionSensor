#include <iostream>
#include <string>
#include <stdio.h>
#include "storageHandler.h"
#include "libsps30.h"
#include "neo6m.h"


class stateMachine {
    public:
	/**
	 * Constructor
	 **/
	stateMachine();

	/**
	 * Destructor 
	 **/
	~stateMachine();

    /**
	 * initiate machine ***RUN ONCE ONLY***
	 **/
    void initiate();

    /**
	 * shutdown machine
	 **/
    void shutdown();

    /**
	 * start data acquisition
	 **/
    void startAcquisition();

    /**
	 * stop data acquisition
	 **/
    void stopAcquisition();

    /**
	 * new file with certain identification number
	 **/
    void startNewTripLog(std::string identificationNumber);

private:
    std::string identificationNumber;
    bool gpsFixedFlag;

    

};