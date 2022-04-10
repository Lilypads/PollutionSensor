#include <iostream>
#include <fstream>
#include "neo6m.h"
#include "libsps30.h"

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
	 * create files
	 **/
    void createFiles(int identificationNumber);

    /**
	 * write to files
	 **/
    void writeToFile(SPS30measurement currentMeasurement);

    /**
	 * GPS buffer
	 **/
    neo6mMeasurment bufferMeasurement;

    private:
            /*
            * syntax documentation
            */
            //write only
            //fstream signal_file;
            //read and write
            //ifstream signal_file;

        fstream logdata_file;

}


    