#define BOOST_TEST_MODULE testModuleName
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "libsps30.h"
//BOOST_WARN(1==1); //warn and continue
//BOOST_CHECK(1>0); //fail but continue
//BOOST_REQUIRE(1!=2); //fail and terminate

BOOST_AUTO_TEST_CASE(TestCalcCrc)
{
    uint8_t data[2];
    data[0] = 0xf0;
    data[1] = 0x0f;
    BOOST_CHECK_EQUAL(0xb7, CalcCrc(data));

    data[0] = 0x41;
    data[1] = 0x47;
    BOOST_CHECK_EQUAL(0x56, CalcCrc(data));

    data[0] = 0x41;
    data[1] = 0x56;
    BOOST_CHECK_EQUAL(0x24, CalcCrc(data));

    data[0] = 0x41;
    data[1] = 0x87;
    BOOST_CHECK_EQUAL(0x11, CalcCrc(data));

}
BOOST_AUTO_TEST_CASE(TestBytesToFloat){
    char bdata[4];
    bdata[0] = 0x3f;  
    bdata[1] = 0x80;  
    bdata[2] = 0x00;  
    bdata[3] = 0x00;   
    float fdata = 1;
    BOOST_CHECK_EQUAL(fdata, bytesToFloat(bdata[0],bdata[1],bdata[2],bdata[3]));
}

