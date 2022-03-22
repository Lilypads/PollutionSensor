#include "sps30.h"

void sps30::start(sps30settings settings){
         sps30settings=settings;
         
         if(settings.initPIGPIO){
         int cfg = gpioCfgGetInternals();
         cfg |= PI_CFG_NOSIGHANDLER;  // (1<<10)
         gpioCfgSetInternals(cfg);
         int status = gpioInitialise();
         if(status<0){
               char msg[] = "cannot init pigpio.";

#ifdef DEBUG
               fprintf(stderr, "%s\n" msg);
#endif        
               throw msg;
            }
    }
# idef DEBUG 
           fprint(stderr, "Init./n");
#endif

// PARAMETERS = DEVICE SETTINGS

        gpioSetMode(unsigned gpio,PI_INPUT);
        gpioSetISRFuncEx(unsigned gpio,unsigned edge,ISR_TIMEOUT,gpioISR,(void*)this);

}
void sps30::dataReady() {
	//read measured value = data 
       hassample(data)
}


void sps30::stop() {
	gpioSetISRFuncEx(ads1115settings.drdy_gpio,RISING_EDGE,-1,NULL,(void*)this);
        if (ads1115settings.initPIGPIO) {
                gpioTerminate();
        }
} 

datatype sps30::STOP_MEASUREMENT(param?){
   //define param
   i2c_write(SPS_STOP_MEASUREMENT)
} 
datatype sps30::READ_DATA_READY_FLAG(param?) {
   //define param
   i2c_write(READ DATA-READY FLAG)
   data= i2c_read(SPS_NBYTES_READ_DATA_READY_FLAG)
   //check crc
}
datatype sps30::START_MEASUREMENT(param?) {
   //define param
   i2c_write(SPS_STOP_MEASUREMENT)  
}
datatype sps30::READ_MEASUREMED_VALUE(param?) {
    //define param
    i2c_write(READ_MEASURED_VALUES)
    data = i2c_read(NBYTES_MEASURED_VALUES_FLOAT)
    //can use keyboard interrupt to stop or button
}
datatype sps30::SLEEP(param?) {
    //define param
    i2c_write(SPS_SLEEP)
}

datatype sps30::WAKE_UP(param?){
    //define param
    i2c_write(SPS_WAKE_UP)
} 
datatype sps30::START_FAN_CLEANING(param?) {
    //define param
    i2c_write(SPS_START_FAN_CLEANING)
}
datatype sps30::AUTOCLEAN_INTERVAL(param?) {
    //define param
    i2c_write(SPS_AUTOCLEAN_INTERVAL)
    data=i2c_read(SPS_NBYTES_AUTO_CLEANING_INTERVAL)
    //assign interval[]
}
datatype sps30::READ_PRODUCT_TYPE(param?) {
   //define param
   i2c_write(SPS_READ_PRODUCT_TYPE)
   data= i2c_read(SPS_NBYTES_READ_PRODUCT_TYPE)
   //check crc
   //return result
}
datatype sps30::SERIAL_NUMBER(param?) {
   //define param
   i2c_write(SPS_SERIAL_NUMBER)
   data= i2c_read(SPS_SERIAL_NUMBER)
   //check crc
   //return result
}
datatype sps30::READ_VERSION(param?){
   //define param
   i2c_write(SPS_READ VERSION)
   data= i2c_read(SPS_READ VERSION)
   //check crc
   //return result
}
datatype sps30::READ_DEVICE_STATUS_REG(param?) {
   //define param
   i2c_write(SPS_READ VERSION)
   data= i2c_read(SPS_READ VERSION)
   //check crc
   //define speed status, laser status, fan status
   //return status
}
datatype sps30::RESET(param?){ 
   i2c_write(SPS_RESET)
}

datatype sps30:CRC(){

}

//i2c read write protocol   
void sps30::i2c_write(param?){
      
      int fd= i2cOpen(unsigned i2cBus, unsigned i2cAddr, 0)
      if(fd>0){
#ifdef DEBUG
		fprintf(stderr,"Could not write\n");
#endif
		throw could_not_open_i2c;
	}
	i2cWriteByteData(fd, unsigned i2c reg, data);
	i2cClose(fd);
}
void sps30::i2c_read(param?){   
       int fd = i2cOpen(unsigned i2cBus, unsigned i2cAddr, 0);
	 if (fd < 0) {
#ifdef DEBUG
		fprintf(stderr,"Could not read byte\n");
		throw could_not_open_i2c;
	}
	int data; 
	data = i2cReadByteData(fd, unisgned i2cReg);
	if (data < 0) {
#ifdef DEBUG
		fprintf(stderr,"Could not read byte.\n");
		throw "Could not read from i2c.";
	}
	i2cClose(fd);
	return data;           
}
