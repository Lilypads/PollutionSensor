#include "storageHandler.h"


// constructor
storageHandler::storageHandler(){
}

//destructor
storageHandler::~storageHandler(){
    closeFiles();
};

void storageHandler::closeFiles(){
    logdata_file.close();
};

void storageHandler::createFiles(int identificationNumber){

    logdata_file.open("./tripLog_" + identificationNumber + ".tsv", fstream::out);

//write header
logdata_file    <<"SPS_MassConcentrationPM1.0" 
                <<"SPS_MassConcentrationPM2.5" 
                <<"SPS_MassConcentrationPM4.0" 
                <<"SPS_MassConcentrationPM10.0" 
                <<"SPS_NumberConcentrationPM0.5" 
                <<"SPS_NumberConcentrationPM1.0" 
                <<"SPS_NumberConcentrationPM2.5" 
                <<"SPS_NumberConcentrationPM4.0" 
                <<"SPS_NumberConcentrationPM10.0" 
                <<"SPS_TypicalParticleSize" 
                <<"GPS_LattitudeDegree"         
                <<"GPS_LongtitudeDegree"
                <<"GPS_HorizontalDilutionOfPosition"
                <<"GPS_AltitudeMetre"
                <<"GPS_UniversalTimeCode"
                <<"GPS_TimeLastUpdate"
                <<"GPS_FixQuality"
                <<"\n"
                << endl;

};

void storageHandler::writeToFile(SPS30measurement currentMeasurement){
logdata_file    << currentMeasurement.MassConcPM1_0
                << currentMeasurement.MassConcPM2_5
                << currentMeasurement.MassConcPM3_0
                << currentMeasurement.MassConcPM10_0
                << currentMeasurement.NumConcPM0_5
                << currentMeasurement.NumConcPM1_0
                << currentMeasurement.NumConcPM2_5
                << currentMeasurement.NumConcPM4_0
                << currentMeasurement.NumConcPM10_0
                << currentMeasurement.TypicalParcSize
                << bufferMeasurement.latt_deg
                << bufferMeasurement.long_deg
                << bufferMeasurement.hdop
                << bufferMeasurement.alt_m
                << bufferMeasurement.utc
                << bufferMeasurement.tLastUpdate
                << bufferMeasurement.fixQuality
                <<"\n"
                << endl;
};



/*
*read from file syntax example
*

raw_infile.open("./pRecordingData/Recording" + rcrdng + ".tsv"); //load data
while (!raw_infile.eof()) {
        count += 1;
        //get the data from .tsv files:
        raw_infile >> sample_num >> signal_raw_data >> noise_raw_data >> check_digit;
}
*/