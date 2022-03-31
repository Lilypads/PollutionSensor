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

//include guards
#ifndef TRY1
#define TRY1

static const char could_not_open_i2c[] = "Could not open I2C.\n";

// default address if ADDR is pulled to GND
#define DEFAULT_SPS30_ADDRESS 0x69

#define DUMMY 0x00
#define START_MEASUREMENT 0x0001
#define BIG_ENDIAN_IEEE754_FLOAT_TYPE 0x03
#define STOP_MEASUREMENT 0x0104
#define READ_VERSION 0xD100


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

    SPS30settings getSPS30settings() const {
		return settings;
    }

	/**
	 * Starts the data acquisition in the background and the
	 * callback is called with new samples.
	 * \param settings A struct with the settings.
	 **/
   // SPS30settings customsettings;
    // customsettings.address =5;
	void start(SPS30settings settings = SPS30settings());

    void readVersion();

	/**
	 * Stops the data acquistion
	 **/
	void stop();
    //void stop();

private:
	SPS30settings settings;

};

#endif
