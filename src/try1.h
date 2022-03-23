#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <assert.h>

// enable debug messages and error messages to stderr
#ifndef NDEBUG
#define DEBUG
#endif

static const char could_not_open_i2c[] = "Could not open I2C.\n";

// default address if ADDR is pulled to GND
#define DEFAULT_SPS30_ADDRESS 0x3c

#define DUMMY 0x00
#define START_MEASUREMENT_B1 0x00
#define START_MEASUREMENT_B2 0x01
#define BIG_ENDIAN_IEEE754_FLOAT_TYPE 0x03

#define STOP_MEASUREMENT_B1 0x01
#define STOP_MEASUREMENT_B2 0x04

#define READ_VERSION_B1 0xD1
#define READ_VERSION_B2 0x00


/**
 * initial settings when starting the device.
 **/
struct SPS30settings {

	/**
	 * I2C bus used (99% always set to one)
	 **/
	int i2c_bus = 1;

	/**
	 * I2C address of the sps30
	 **/
	uint8_t address = DEFAULT_SPS30_ADDRESS;


};

class SPS30 {

public:
	/**
	 * Destructor which makes sure the data acquisition
	 * stops on exit.
	 **/
	~SPS30() {
		stop();
	}



	/**
	 * Starts the data acquisition in the background and the
	 * callback is called with new samples.
	 * \param settings A struct with the settings.
	 **/
	void start(SPS30settings settings);

    void readVersion(SPS30settings settings);

	/**
	 * Stops the data acquistion
	 **/
	//void stop(SPS30settings settings);
    void stop();

};