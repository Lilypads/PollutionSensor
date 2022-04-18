#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <assert.h>
#include <cstring>

// enable debug messages and error messages to stderr
#ifndef NDEBUG
#define DEBUG
#endif
static const char could_not_open_i2c[] = "Could not open I2C.\n";
#define DEFAULT_SSD1306_ADDRESS 0x3D



#define SSD1306_MEMORYMODE 0x20          ///< See datasheet
#define SSD1306_COLUMNADDR 0x21          ///< See datasheet
#define SSD1306_PAGEADDR 0x22            ///< See datasheet
#define SSD1306_SETCONTRAST 0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON 0xA5        ///< Not currently used
#define SSD1306_NORMALDISPLAY 0xA6       ///< See datasheet
#define SSD1306_INVERTDISPLAY 0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8        ///< See datasheet
#define SSD1306_DISPLAYOFF 0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 0xAF           ///< See datasheet
#define SSD1306_COMSCANINC 0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC 0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SSD1306_SETPRECHARGE 0xD9        ///< See datasheet
#define SSD1306_SETCOMPINS 0xDA          ///< See datasheet
#define SSD1306_SETVCOMDETECT 0xDB       ///< See datasheet

#define SSD1306_SETLOWCOLUMN 0x00  ///< Not currently used
#define SSD1306_SETHIGHCOLUMN 0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE 0x40  ///< See datasheet

#define SSD1306_EXTERNALVCC 0x01  ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

//not currently used
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range



#if defined SSD1306_128_32
#define SSD1306_WIDTH 128 ///< DEPRECATED: width w/SSD1306_128_32 defined
#define SSD1306_HEIGHT 32 ///< DEPRECATED: height w/SSD1306_128_32 defined

#endif

struct SSD1306settings {

	/**
	 * I2C bus used (99% always set to one)
	 **/
	uint8_t i2c_bus = 1;

	/**
	 * I2C address of the SSD1306
	 **/
	uint8_t address = DEFAULT_SS1306_ADDRESS;


 	/**
	 * If set to true the pigpio will be initialised
	 **/
	bool initPIGPIO = true;

};
class SSSD1306 {
    public :
    /**
	 * Constructor
	 **/
    SSD1306(SSD1306settings settings = SSD1306settings());
    /**
	 * Destructor 
	 * stops on exit.
	 **/
	~SSD1306();

	/**
  	 * method to get current settings.
     **/
    SSD1306settings getSSD1306settings() const {
		return settings;
    }
    bool begin	(uint8_t vcs = SSD1306_SWITCHCAPVCC );
    void initialization();
    void display(void);
    void clearDisplay(void);
    void data(uint8_t c);
    uint8_t *getBuffer(void);

protected:
  void command(uint8_t c);


  int8_t vccstate; ///< VCC selection, set by begin method.
  uint8_t *buffer;


};
#endif
