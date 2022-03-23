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
#define DEFAULT_SPS30_ADDRESS 0x48

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

    /**
	 * If set to true the pigpio will be initialised
	 **/
	bool initPIGPIO = true;
};

class SPS30rpi {

public:
	/**
	 * Destructor which makes sure the data acquisition
	 * stops on exit.
	 **/
	~SPS30rpi() {
		stop();
	}

	/**
	 * Called when a new sample is available.
	 * This needs to be implemented in a derived
	 * class by the client. Defined as abstract.
	 * \param sample Voltage from the selected channel.
	 **/
	virtual void hasSample(float sample) = 0;

    	/**
	 * Starts the data acquisition in the background and the
	 * callback is called with new samples.
	 * \param settings A struct with the settings.
	 **/
	void start(ADS1115settings settings = ADS1115settings() );

    

    private:
	SPS30settings sps30settings;
    void dataReady();


	void i2c_writeWord(uint8_t reg, unsigned data);
        unsigned i2c_readWord(uint8_t reg);
        int i2c_readConversion();