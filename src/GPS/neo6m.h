#include <stdint.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>


#define NMEA_MAX_SENTENCE_SIZE 79+1 //+1 for null terminator
#define NMEA_MAX_DATA_FIELD_SIZE 74 //+1 for null terminator
#define CIRC_BUFF_SIZE NMEA_MAX_SENTENCE_SIZE*2
#define NMEA_SENTENCE_START_DELIM '$'
#define NMEA_SENTENCE_END_DELIM '\n'
#define NMEA_SENTENCE_CHECKSUM_DELIM '*'
#define NMEA_SENTANCE_TYPE_lENGTH 5
#define NMEA_CHECKSUM_LENGTH 2
#define NMEA_SENTENCE_DATA_DELIM ','
#define NMEA_SENTENCE_MIN_SIZE 20 //this is just based on what ive seen
#define NMEA_MAX_SENTANCES_IN_BUFF CIRC_BUFF_SIZE/NMEA_SENTENCE_MIN_SIZE
#define SERIAL_DEVICE_STR_LEN 20
#define NMEA_MAX_TRANMISSION_INTERVAL_S 2


int hexChar2IntLUT[] =
    {
// ASCII
//0  1   2   3   4   5   6   7   7   8   10 11  12  13  14  15
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0-15
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 16-31
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 32-47
 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, // 48-63
-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 64-79
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 80-95
-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 96-111
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 112-127
};


int decChar2IntLUT[] =
    {
// ASCII
//0  1   2   3   4   5   6   7   7   8   10 11  12  13  14  15
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0-15
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 16-31
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 32-47
 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, // 48-63
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 64-79
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 80-95
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 96-111
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 112-127
};

#ifndef NEO6M_H_
#define NEO6M_H_

struct neo6mSettings {
    std::string serialDevice = "/dev/serial0";
    int baudrate = 9600;
};

struct neo6mMeasurment{
    float latt_deg;
    float long_deg;
    float alt_m;
    float heading_deg;
    float speed_mps;
    float error2d_m;
    float error3d_m;
    float errorSpd_m;
    bool dropout;
};

class NEO6M {
//public properties
public:
    //constructor
    NEO6M(neo6mSettings theseSettings = neo6mSettings());
    //destructor
//    ~NEO6M();

    void startMeasurement();
    void stopMeasurement();

    neo6mMeasurment lastCompleteSample;
    uint timeLastSample;
    //user must implement this method!
    virtual void hasMeasurementCB(neo6mMeasurment thisMesurement) = 0;

//private methods
private:
    int fd = -1; //file handle
    void hasNmeaSentance(int length); // TODO //the polling method will call this once a full sentance has been recieved
    int configurePort(int fd, int baud); // DONE
    bool testChecksum(char* sentance); // DONE
    int hexChar2Int(char* checksumChar); // DONE
    void pollUartDev(); // TODO
    int parseNmeaStr(std::array<char,NMEA_SENTANCE_TYPE_lENGTH> &sentenceType, std::array<char,NMEA_MAX_DATA_FIELD_SIZE/2> &data, std::array<char,NMEA_CHECKSUM_LENGTH>  &checksum); // TODO void pollUartDev(); // TODO
    static void execPollingThread(NEO6M* thisClassPtr){
        thisClassPtr->pollUartDev();
    }
    bool isPollingUart=0;
    std::thread* daqThread = nullptr;
    neo6mSettings settings;

    std::array<char, NMEA_MAX_SENTENCE_SIZE> nmeaSentence;

    std::array<char, NMEA_SENTANCE_TYPE_lENGTH> sentenceType;
    std::array<char, NMEA_MAX_DATA_FIELD_SIZE/2> data;
    std::array<char, NMEA_CHECKSUM_LENGTH> checksum;

};


#endif // NEO6M_H_
