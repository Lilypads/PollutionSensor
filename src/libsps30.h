#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <assert.h>
#include <cstring>
#include <thread>

// enable debug messages and error messages to stderr
#ifndef NDEBUG
#define DEBUG
#endif

// enable no hardware mode
#ifndef N_NO_HARDWARE
#define NO_HARDWARE
#endif

//include guards
#ifndef TRY1
#define TRY1

//define constants
#define DRDY_POLLINGPERIOD_US 50000 //every 0.050s
#define SN_LEN_W_SRC 48 //serial number data length
#define SN_LEN_WO_SRC SN_LEN_W_SRC*2/3 //serial number read data loops: this is because there is a checksum byte every 2 data bytes

static const char could_not_open_i2c[] = "Could not open I2C.\n";

//default address if ADDR is pulled to GND (i2c mode) according to SENSIRION data sheet
#define DEFAULT_SPS30_ADDRESS 0x69

//pointers to address on the device to do actions according to SENSIRION data sheet
#define DUMMY 0x00
#define BIG_ENDIAN_IEEE754_FLOAT_TYPE 0x03
#define START_MEASUREMENT 0x0010
#define STOP_MEASUREMENT 0x0104

#define READ_DRDY_FLAG 0x0202
#define READ_MEASURED_VALUES 0x0300

#define READ_VERSION 0xD100
#define READ_SERIAL_NUMBER 0xD033

//other pointers not yet implemented on cpp
#define START_FAN_CLEANING 0x5607
#define READWRITE_AUTOCLEANING_INTERVAL 0x8004
#define READ_DEVICE_STATUS_REGISTER 0xD206
#define CLEAR_DEVICE_STATUS_REGISTER 0xD210
#define RESET 0xD304


/**
 * initial settings when starting the device.
 **/
struct SPS30settings {

	/**
	 * I2C bus used (99% always set to one)
	 **/
	uint8_t i2c_bus = 1;

	/**
	 * I2C address of the sps30
	 **/
	uint8_t address = DEFAULT_SPS30_ADDRESS;


 	/**
	 * If set to true the pigpio will be initialised
	 **/
	bool initPIGPIO = true;

	/**
	 * auto start thread flag
	 **/
	bool autoStartThread = true;


};


/**
 * indices for parsing read measurement data.
 **/
enum SPS30dataOutputIdx{
		//unit ug/m^3
		idxMassConcPM1_0 = 0,  
		idxMassConcPM2_5 = 6,
		idxMassConcPM4_0 = 12,
		idxMassConcPM10_0 = 18,
		//unit #/cm^3
		idxNumConcPM0_5 = 24,
		idxNumConcPM1_0 = 30,
		idxNumConcPM2_5 = 36,
		idxNumConcPM4_0 = 42,
		idxNumConcPM10_0 = 48,
		//unit um
		idxTypicalParcSize = 54
};

/**
 * contain read measurement of the device.
 **/
struct SPS30measurement {

	float MassConcPM1_0;
	float MassConcPM2_5;
	float MassConcPM4_0;
	float MassConcPM10_0;
	float NumConcPM0_5;
	float NumConcPM1_0;
	float NumConcPM2_5;
	float NumConcPM4_0;
	float NumConcPM10_0;
	float TypicalParcSize;

};

/**
* function to calculate checksum for data communication.
**/
uint8_t CalcCrc(uint8_t data[2]);

/**
* function to change 4 bytes data into floating value.
**/
float bytesToFloat(char b0, char b1, char b2, char b3);

class SPS30 {

public:
	/**
	 * Constructor
	 **/
	SPS30(SPS30settings settings = SPS30settings());

	/**
	 * Destructor which makes sure the data acquisition
	 * stops on exit.
	 **/
	~SPS30();

	/**
  	 * method to get current settings.
     **/
    SPS30settings getSPS30settings() const {
		return settings;
    }

	/**
  	 * The user must implement this callback in the inherited class.
     **/
	virtual void hasMeasurmentCB(SPS30measurement measurement) = 0;
	
	/**
	 * Starts the data acquisition in the background thread and the
	 * callback is called with new samples.
	 **/
	void startMeasurement(SPS30settings settings = SPS30settings());

	/**
  	 * reads out firmware version.
     **/
    void readVersion(); // TODO this doesnt return anything unlike other fucntions would be good to have only an optional print

	/**
  	 * reads out data ready flag.
     **/
    int readDRDYFlag();

	/**
  	 * reads out serial number.
     **/
    int readSerialNumber(); // TODO this currently only reads the serial number to a class property!!

	/**
  	 * reads out latest measurement.
     **/
	SPS30measurement readMeasurement(); // this spits out the mesurement which i think is the right thing to do

	/**
	 * Stops the data acquistion.
	 **/
	void stop();

	/**
	 * serialnumber string.
	 **/
	char serialNumber[SN_LEN_W_SRC]="";

		/**
	 * run method of the thread.
	 **/
	void pollDRDYFlag();

private:

	/**
	 * initiate default setting.
	 **/
	SPS30settings settings;

	/**
	 * running flag.
	 **/
	bool isPollingDRDY=0;

	/**
	 * threading in the background.
	 **/
	static void execPollingThread(SPS30* thisClassPtr){
	thisClassPtr->pollDRDYFlag();
	}
	std::thread* daqThread = nullptr;


};

#endif
