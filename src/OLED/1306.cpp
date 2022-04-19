#include "SSD1306.h"
SSD1306::SSD1306(SSD1306settings userSettings){
    settings = userSettings;

if(settings.initPIGPIO){
         int cfg = gpioCfgGetInternals();
         cfg |= PI_CFG_NOSIGHANDLER;  
         gpioCfgSetInternals(cfg);
         int status = gpioInitialise();
         if(status<0){
               char msg[] = "cannot init pigpio.";
            }
    }
   
}
void SSD1306::setSettings(SSD1306settings newSettings){
settings = newSettings;
};

//destructor
SSD1306::~SSD1306(){
    stop();
};

bool SSD1306::begin	(uint8_t vcs = SSD1306_SWITCHCAPVCC ){
vccstate = vcs;
int handle = i2cOpen(settings.i2c_bus,settings.address,0);
if (handle<0){
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c; 
}
init();
command(SSD1306_DISPLAYON);
}

void SSD1306::init(){
        //128x32 pixel specific initialization.
        command(SSD1306_DISPLAYOFF);                   # 0xAE
        command(SSD1306_SETDISPLAYCLOCKDIV);            # 0xD5
        command(0x80);                                  # the suggested ratio 0x80
        command(SSD1306_SETMULTIPLEX);                  # 0xA8
        command(0x1F);
        command(SSD1306_SETDISPLAYOFFSET) ;             # 0xD3
        command(0x0);                                   # no offset
        command(SSD1306_SETSTARTLINE | 0x0);            # line #0
        command(SSD1306_CHARGEPUMP);                # 0x8D
        if(vccstate == SSD1306_EXTERNALVCC)
            command(0x10);
        else
           command(0x14);
        command(SSD1306_MEMORYMODE) ;                   # 0x20
        command(0x00)     ;                             # 0x0 
        command(SSD1306_SEGREMAP | 0x1);
        command(SSD1306_COMSCANDEC);
        command(SSD1306_SETCOMPINS) ;                   # 0xDA
        command(0x02);
        command(SSD1306_SETCONTRAST)  ;                 # 0x81
        command(0x8F);
        command(SSD1306_SETPRECHARGE) ;                 # 0xd9
        if(vccstate == SSD1306_EXTERNALVCC)
            command(0x22);
        else
            command(0xF1);
        command(SSD1306_SETVCOMDETECT);                 # 0xDB
        command(0x40);
        command(SSD1306_DISPLAYALLON_RESUME)  ;         # 0xA4
        command(SSD1306_NORMALDISPLAY) ;                # 0xA6
}

void SSD1306::display(void)	{
      command(SSD1306_COLUMNADDR);
      command(0); // start address
      command(SSD1306_WIDTH-1); // end address
      command(SSD1306_PAGEADDR);
      command(0); // start address
      command(3); // end address
      
uint16_t count =  SSD1306_WIDTH * ((SSD1306_HEIGHT + 7) / 8);
uint8_t *ptr = buffer;
int handle = i2cOpen(settings.i2c_bus,settings.address,0);
if (handle<0){
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c; 

}

int checkERR = i2cWriteByte(handle,(uint8_t)0x40);
    if(checkERR<0){
      #ifdef DEBUG
                fprintf(stderr,"Cannot write byte.\n");

      #endif
        
    }
uint16_t bytesOut = 1;
while(count--){
    i2cWriteByte(handle,(uint8_t)0x40);
    bytesOut=1;
}
i2cWriteByte(handle,*ptr++);
bytesOut++;
i2cClose(handle);
}

//
void SSD1306::command(uint8_t c){
int handle = i2cOpen(settings.i2c_bus,settings.address,0);
if (handle<0){
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c; 
}
unsigned char buf[2];
buf[0] =0x00;
buf[1] =c;
int checkERR = i2cWriteDevice(handle,buf,2);
    if(checkERR<0){
      #ifdef DEBUG
                fprintf(stderr,"Cannot write byte.\n");

      #endif
    }
    i2cClose(handle);
}
//d
void SSD1306::data(uint8_t c){
int handle = i2cOpen(settings.i2c_bus,settings.address,0);
if (handle<0){
#ifdef DEBUG
                fprintf(stderr,"Could not open %02x.\n",settings.address);
#endif
                throw could_not_open_i2c; 
}
unsigned char buf[2];
buf[0]= 0x40;
buf[1] =c;
int checkERR = i2cWriteDevice(handle,buf,2);
    if(checkERR<0){
      #ifdef DEBUG
                fprintf(stderr,"Cannot write byte.\n");

      #endif
    }
    i2cClose(handle);
}

void SSD1306::clearDisplay	(void){
 memset(buffer, 0, SSD1306_WIDTH * ((SSD1306_HEIGHT + 7) / 8));
 	cursor_y = 0;
	cursor_x = 0;
}




//invert display
void SSD1306::invertDisplay(unsigned int i)
{
	if (i) {
		command(SSD1306_INVERTDISPLAY);
	} else {
		command(SSD1306_NORMALDISPLAY);
	}
}
//scroll right
void SSD1306::startscrollright(unsigned int start, unsigned int stop)
{
	command(SSD1306_RIGHT_HORIZONTAL_SCROLL);
	command(0X00);
	command(start);
	command(0X00);
	command(stop);
	command(0X00);
	command(0XFF);
	command(SSD1306_ACTIVATE_SCROLL);
}

// startscrollleft

void SSD1306::startscrollleft(unsigned int start, unsigned int stop)
{
	command(SSD1306_LEFT_HORIZONTAL_SCROLL);
	command(0X00);
	command(start);
	command(0X00);
	command(stop);
	command(0X00);
	command(0XFF);
	command(SSD1306_ACTIVATE_SCROLL);
}

// startscrolldiagright

void SSD1306::startscrolldiagright(unsigned int start, unsigned int stop)
{
	command(SSD1306_SET_VERTICAL_SCROLL_AREA);
	command(0X00);
	command(SSD1306_HEIGHT);
	command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
	command(0X00);
	command(start);
	command(0X00);
	command(stop);
	command(0X01);
	command(SSD1306_ACTIVATE_SCROLL);
}

// startscrolldiagleft

void SSD1306::startscrolldiagleft(unsigned int start, unsigned int stop)
{
	command(SSD1306_SET_VERTICAL_SCROLL_AREA);
	command(0X00);
	command(SSD1306_HEIGHT);
	command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
	command(0X00);
	command(start);
	command(0X00);
	command(stop);
	command(0X01);
	command(SSD1306_ACTIVATE_SCROLL);
}

//stop scroll
void SSD1306::stopscroll(void)
{
	command(SSD1306_DEACTIVATE_SCROLL);
}

                
 







