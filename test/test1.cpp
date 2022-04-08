#define BOOST_TEST_MODULE testModuleName
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "libsps30.h"
//BOOST_WARN(1==1); //warn and continue
//BOOST_CHECK(1>0); //fail but continue
//BOOST_REQUIRE(1!=2); //fail and terminate

#define NO_HARDWARE



BOOST_AUTO_TEST_CASE(TestDefaultSettings)
{
    class SPS30Tester: public SPS30 {
    virtual void hasMeasurmentCB(SPS30measurement thisMeasurement){
        BOOST_CHECK_EQUAL(2.345,thisMeasurement.MassConcPM1_0);
	    BOOST_CHECK_EQUAL(8.91,thisMeasurement.TypicalParcSize);
        }
    };

    SPS30settings testsettings;
    SPS30Tester testsps30;
    testsettings = testsps30.getSPS30settings();
    BOOST_CHECK_EQUAL(DEFAULT_SPS30_ADDRESS, testsettings.address);
    BOOST_CHECK_EQUAL(1, testsettings.i2c_bus);
    BOOST_CHECK_EQUAL(true, testsettings.initPIGPIO);
    BOOST_CHECK_EQUAL(true, testsettings.autoStartThread);
}

BOOST_AUTO_TEST_CASE(TestFunctions)
{
    uint8_t data[2];
    data[0] = 0xf0;
    data[1] = 0x0f;
    BOOST_CHECK_EQUAL(0x00, CalcCrc(data));

    char bdata[4];
    bdata[0] = 0x3f;  
    bdata[1] = 0x80;  
    bdata[2] = 0x00;  
    bdata[3] = 0x00;   
    float fdata = 1;
    BOOST_CHECK_EQUAL(fdata, bytesToFloat(bdata[0],bdata[1],bdata[2],bdata[3]));
}


BOOST_AUTO_TEST_CASE(TestClassMethods)
{
    class SPS30Tester: public SPS30 {
    virtual void hasMeasurmentCB(SPS30measurement thisMeasurement){
        BOOST_CHECK_EQUAL(2.345,thisMeasurement.MassConcPM1_0);
	    BOOST_CHECK_EQUAL(8.91,thisMeasurement.TypicalParcSize);
        }
    };

    SPS30Tester testsps30;
    SPS30settings testsettings;
    testsettings.initPIGPIO = false;
    testsettings.autoStartThread = false;

    testsps30.startMeasurement();
    testsps30.readVersion();
    BOOST_CHECK_EQUAL(1,testsps30.readSerialNumber());

    testsps30.pollDRDYFlag();
    testsps30.stop();
}


