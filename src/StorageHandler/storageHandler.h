#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "neo6m.h"
#include "libsps30.h"

//include guards
#ifndef STORAGEHANDLER
#define STORAGEHANDLER


class storageHandler {
    public:
	/**
	 * Constructor
	 **/
	storageHandler();

	/**
	 * Destructor 
	 **/
	~storageHandler();

    /**
	 * close all files
	 **/
    void closeFiles();

    /**
	 * create files headers _PLEASE RUN ONLY ONCE OTHERWISE IT OVERWRITES_
	 **/
    void createFiles();

    /**
	 * write to files
	 **/
    void addMeasurement(SPS30measurement currentMeasurement);

    /**
	 * GPS buffer
	 **/
    neo6mMeasurment bufferMeasurement;

    /**
	 * identification number that goes into the saved file name
	 **/
    std::string identificationNumber = "0";
    
    private:

            /*
            * syntax documentation
            */
            //write only
            //fstream signal_file;
            //read and write
            //ifstream signal_file;

    std::fstream logdata_file;

};

#endif


    
