#define BOOST_TEST_MODULE testModuleName
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "libsps30.h"
//BOOST_WARN(1==1); //warn and continue
//BOOST_CHECK(1>0); //fail but continue
//BOOST_REQUIRE(1!=2); //fail and terminate

BOOST_AUTO_TEST_CASE(TestDefaultSettings)
{
    SPS30 testsps30;
    SPS30settings testsettings
    testsettings = testsps30.getsps30settings();
    BOOST_CHECK_EQUAL(DEFAULT_SPS30_ADDRESS, testsettings.address);
    BOOST_CHECK_EQUAL(1, testsettings.bus);
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
    bdata[0] = 0xf0;  
    bdata[0] = 0xf0;  
    bdata[0] = 0xf0;  
    bdata[0] = 0xf0;   
    float fdata = ;
    BOOST_CHECK_EQUAL(fdata, bytesToFloat(bdata));
}

BOOST_AUTO_TEST_CASE(Testname)
{
    BOOST_CHECK_EQUAL(4, sqr(2));
}
