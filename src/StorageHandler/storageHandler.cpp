#include "storageHandler.h"


// constructor
storageHandler::storageHandler(){
};

//destructor
storageHandler::~storageHandler(){
    closeFiles();
};

void storageHandler::closeFiles(){
    logdata_file.close();
};

std::string storageHandler::getTimeAsStr(){
    //Set the current time
    time_t secs = time(0);

    //Convert the current time into a struct containing years, months, days, hours etc
    struct tm* local = localtime(&secs);

    //Convert the struct into a string to be returned
    return std::to_string(local->tm_year) + "-" + std::to_string(local->tm_mon) + "-" + std::to_string(local->tm_mday) + "_" + std::to_string(local->tm_hour) + "-" + std::to_string(local->tm_min);

};

int storageHandler::GetUniqueFileNumber()
{
    //Set the ID to the defualt 1
    int checkNum = 1;

    //Set the name of the first file to check
    std::string name = fileName + std::to_string(checkNum) + ".csv";

    //Try to open the first file
    FILE *fileCheck = fopen(name.c_str(), "r");

    //If the file could be opened
    while(fileCheck)
    {
        //Close the file, because we now know it exists and that's all we needed
        fclose(fileCheck);

        //Increment the ID
        checkNum++;

        //Set the name of the next file to check
        name = fileName + std::to_string(checkNum) + ".csv";

        //Try to open the next file
        fileCheck = fopen(name.c_str(), "r");
    };

    return checkNum;    //Return the first ID that did not already have a file associated with it
};

void storageHandler::createFiles(){

//Set the base for the filename
fileName = "tripLog_" + getTimeAsStr() + "_";

//Get the unique file ID
identificationNumber = std::to_string(GetUniqueFileNumber());

//Complete the filename
fileName = fileName + identificationNumber + ".csv";

//Set the save directory
saveDirectory = "/var/lib/polsense/triplogs/";

//Open the file to save data to it
logdata_file.open(saveDirectory + fileName, std::fstream::out);

//write header
logdata_file    <<"SPS_MassConcentrationPM1.0" << ","
                <<"SPS_MassConcentrationPM2.5" << ","
                <<"SPS_MassConcentrationPM4.0" << ","
                <<"SPS_MassConcentrationPM10.0" << ","
                <<"SPS_NumberConcentrationPM0.5" << ","
                <<"SPS_NumberConcentrationPM1.0" << ","
                <<"SPS_NumberConcentrationPM2.5" << ","
                <<"SPS_NumberConcentrationPM4.0" << ","
                <<"SPS_NumberConcentrationPM10.0" << ","
                <<"SPS_TypicalParticleSize" << ","
                <<"GPS_LattitudeDegree" << ","
                <<"GPS_LongtitudeDegree" << ","
                <<"GPS_HorizontalDilutionOfPosition" << ","
                <<"GPS_AltitudeMetre"  << ","
                <<"GPS_UniversalTimeCode"  << ","
                <<"GPS_TimeLastUpdate"  << ","
                <<"GPS_FixQuality"
                << std::endl;

//    logdata_file.close();

};

void storageHandler::addMeasurement(SPS30measurement currentMeasurement){

//    logdata_file.open("./tripLog_" + identificationNumber + ".tsv", std::fstream::out);

logdata_file    << currentMeasurement.MassConcPM1_0<< ","
                << currentMeasurement.MassConcPM2_5<< ","
                << currentMeasurement.MassConcPM4_0<< ","
                << currentMeasurement.MassConcPM10_0<< ","
                << currentMeasurement.NumConcPM0_5<< ","
                << currentMeasurement.NumConcPM1_0<< ","
                << currentMeasurement.NumConcPM2_5<< ","
                << currentMeasurement.NumConcPM4_0<< ","
                << currentMeasurement.NumConcPM10_0<< ","
                << currentMeasurement.TypicalParcSize<< ","
                << bufferMeasurement.latt_deg<< ","
                << bufferMeasurement.long_deg<< ","
                << bufferMeasurement.hdop<< ","
                << bufferMeasurement.alt_m<< ","
                << bufferMeasurement.utc<< ","
                << bufferMeasurement.tLastUpdate<< ","
                << bufferMeasurement.fixQuality
                << std::endl;

//    logdata_file.close();
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
