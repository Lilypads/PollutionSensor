#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testNeo6m
#include <boost/test/unit_test.hpp>
#include <neo6m.h>


char GPVTG[] = "$GPVTG,,T,,M,0.275,N,0.509,K,A*2F/r/n";
char GPGGA[] = "$GPGGA,140138.00,5551.65584,N,00413.16212,W,1,06,2.19,51.1,M,50.8,M,,*7B/r/n";
char GPGSA[] = "$GPGSA,A,3,27,16,09,04,26,31,,,,,,,2.83,2.19,1.79*07/r/n";;
char GPGSV[] = "$GPGSV,3,1,12,04,08,283,25,05,20,045,,09,10,315,22,16,54,288,36*72/r/n";
char GPGLL[] = "$GPGLL,5551.65584,N,00413.16212,W,140138.00,A,A*7A/r/n";
char GPRMC[] = "$GPRMC,140139.00,A,5551.65598,N,00413.16265,W,0.164,,060422,,,A*6E/r/n";

// define an inherited class which implements the has sample method
// And can also access private members for testing
class neo6mTester: public NEO6M {
public:
  virtual void hasMeasurementCB(neo6mMeasurment thisMesurement) {
    fprintf(stderr, "Doing nothing for now!\n");
  };

    bool test_testChecksum(char* sent2test,int size){
        // the sentance is a prerecorded NMEA string so the checksum test should pass
        bool out = testChecksum(sent2test);
        return out;
    };
    int test_hexChar2Int(char* checksumChar){
        int out = hexChar2Int(checksumChar);
        return out;
    }
};


//BOOST_WARN(1==1); //warn and continue
//BOOST_CHECK(1>0); //fail but continue
//BOOST_REQUIRE(1!=2); //fail and terminate

BOOST_AUTO_TEST_CASE(PassTest)
{
BOOST_REQUIRE(true);
neo6mTester testNeo6m;

char testSent[sizeof(GPGSA)];
// memcpy(testSent,gpgsa,sizeof(dummyNmeaSent.gpgsa));

BOOST_CHECK(testNeo6m.test_hexChar2Int(GPGSA));
//instantiate neo6m

// test
}

//BOOST_AUTO_TEST_CASE(FailTest)
//{
//    BOOST_CHECK_EQUAL(4, sqr(3));
//}
