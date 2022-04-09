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
#include <math.h>


#define NMEA_MAX_SENTENCE_SIZE 79+1 //+1 for null terminator
#define NMEA_MAX_DATA_FIELD_SIZE 12//+1 for null terminator
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
#define NMEA_END_OF_SENT_lENGTH 5
#define NMEA_DATA_START_IDX 6
#define NMEA_MAX_DATA_ARRAY_SIZE 20

// message types
#define NMEA_GPVTG_MSG "GPVTG"//
#define NMEA_GPGGA_MSG "GPGGA"// Global Positioning System Fix Data (t,lat,long,fixQual,Nsat,HDOP,,alt,,)
#define NMEA_GPGSA_MSG "GPGSA"//
#define NMEA_GPGSV_MSG "GPGSV"//GPS Satellites in view
#define NMEA_GPGLL_MSG "GPGLL"//Geographic Position, Latitude / Longitude and time.
#define NMEA_GPRMC_MSG "GPRMC"//

struct gpggaFieldInfo{int idx, size;};

struct gpgsaFields {
  gpggaFieldInfo id = {0, 5};
  gpggaFieldInfo t = {1,9};
  gpggaFieldInfo lat = {2,10};
  gpggaFieldInfo latB = {3,1};
  gpggaFieldInfo lon = {4,10};
  gpggaFieldInfo lonB = {5,1};
  gpggaFieldInfo qual = {6,1};
  gpggaFieldInfo nSat = {7,2};
  gpggaFieldInfo hdop = {8,4};
  gpggaFieldInfo alt = {9,4};
  gpggaFieldInfo altUnit = {9,1};
  gpggaFieldInfo hAboveEllipsoid = {10,4};
  gpggaFieldInfo tLastUpdate = {11,1};
  gpggaFieldInfo statId = {12,0};
};

extern int hexChar2IntLUT[128];
extern int decChar2IntLUT[128];


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
    typedef std::array<char*, NMEA_MAX_DATA_ARRAY_SIZE> parsedNmeaSent;

//private methods
public:
    int fd = -1; //file handle
    void hasNmeaSentance(parsedNmeaSent& parsedSent); // TODO //the polling method will call this once a full sentance has been recieved
    int configurePort(int fd, int baud); // DONE
    int testChecksum(char* sentence); // DONE
    int hexChar2Int(char* checksumChar); // DONE
    int decChar2Int(char* thisCharFloat);
    float decChar2Float(char* thisCharFloat);
    void pollUartDev(); // TODO
    int parseNmeaStr(char* sentence, int size, parsedNmeaSent& outputSentance); // TODO void pollUartDev(); // TODO
    static void execPollingThread(NEO6M* thisClassPtr){
        thisClassPtr->pollUartDev();
    }
    bool isPollingUart=0;
    std::thread* daqThread = nullptr;
    neo6mSettings settings;

    };

#endif // NEO6M_H_