#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testNeo6m
#define NEO6M_PUB_METHODS //'publisize' neo6m
#include <boost/test/unit_test.hpp>
#include <neo6m.h>

char GPVTG[] = "$GPVTG,,T,,M,0.275,N,0.509,K,A*2F/r/n";
char GPGGA[] = "$GPGGA,140138.00,5551.65584,N,00413.16212,W,1,06,2.19,51.1,M,50.8,M,,*7B/r/n";
char GPGSA[] = "$GPGSA,A,3,27,16,09,04,26,31,,,,,,,2.83,2.19,1.79*07/r/n";
char GPGSA_Error[] = "$GPGSA,A,3,27,16,09,04,26,31,,,,,,,2.82,2.19,1.79*07/r/n";
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

        // the sentance is a prerecorded NMEA string so the checksum test should pass
};

bool nearly_equal(double a, double b)
{
    fprintf(stderr,"[%f,%f]",a,b);
  return std::nextafter(a, std::numeric_limits<double>::lowest()) <= b
    && std::nextafter(a, std::numeric_limits<double>::max()) >= b;
}
//instantiate neo6m
neo6mTester testNeo6m;
//BOOST_WARN(1==1); //warn and continue
//BOOST_CHECK(1>0); //fail but continue
//BOOST_REQUIRE(1!=2); //fail and terminate

BOOST_AUTO_TEST_CASE(PassTest)
{
//BOOST_REQUIRE(true);
// test checksum hex to char conversion
char testHexChar[] = "FF";
BOOST_CHECK_EQUAL(testNeo6m.hexChar2Int(testHexChar),255);
char testHexChar_0[] = "00";
BOOST_CHECK_EQUAL(testNeo6m.hexChar2Int(testHexChar_0),0);
char testHexChar_211[] = "d3";
BOOST_CHECK_EQUAL(testNeo6m.hexChar2Int(testHexChar_211),211);

// test
};

BOOST_AUTO_TEST_SUITE(testChar2Int)

BOOST_AUTO_TEST_CASE(TestChar2Int_8) {
  char testDecChar[] = "8";
  BOOST_CHECK_EQUAL(testNeo6m.decChar2Int(testDecChar), 8);
}
BOOST_AUTO_TEST_CASE(TestChar2Int_7970) {
  char testDecChar_7970[] = "7970";
  BOOST_CHECK_EQUAL(testNeo6m.decChar2Int(testDecChar_7970), 7970);
}
BOOST_AUTO_TEST_CASE(TestChar2Int_ampersand) {
  fprintf(stderr,"Debug message is expected:\n");
  char testDecChar_amper[] = "&";
  BOOST_CHECK_EQUAL(testNeo6m.decChar2Int(testDecChar_amper), -1);
  fprintf(stderr,"\n");
}
BOOST_AUTO_TEST_CASE(TestChar2Int_Plus) {
  fprintf(stderr,"Debug message is expected:\n");
  char testDecChar_plus[] = "+";
  BOOST_CHECK_EQUAL(testNeo6m.decChar2Int(testDecChar_plus), -1);
  fprintf(stderr,"\n");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(testChar2Float)

BOOST_AUTO_TEST_CASE(init_testChar2Float) {
fprintf(stderr,"Testing Char2Float (some debug message are expected):\n");
}
BOOST_AUTO_TEST_CASE(TestChar2Float_8) {
  char testDecChar[] = "0.8";
  BOOST_CHECK(nearly_equal(testNeo6m.decChar2Float(testDecChar), 0.8));
  fprintf(stderr,"\n");
}
BOOST_AUTO_TEST_CASE(TestChar2Float_7970) {
  char testDecChar_7970[] = "7970.";
  BOOST_CHECK(nearly_equal(testNeo6m.decChar2Float(testDecChar_7970), (float) 7970));
  fprintf(stderr,"\n");
}
BOOST_AUTO_TEST_CASE(TestChar2Float_long) {
  char testDecChar_long[] = "9140139.22";
  BOOST_CHECK(nearly_equal(testNeo6m.decChar2Float(testDecChar_long),  9140139.22));
  fprintf(stderr,"\n");
}
BOOST_AUTO_TEST_CASE(TestChar2Float_long2) {
  char testDecChar_long[] = "5551.65584";
  BOOST_CHECK(nearly_equal(testNeo6m.decChar2Float(testDecChar_long),  5551.65584));
  fprintf(stderr,"\n");
}
BOOST_AUTO_TEST_CASE(TestChar2Float_typicalGps) {
  char testDecChar_typicalHPS[] = "00413.16265";
  BOOST_CHECK(nearly_equal(testNeo6m.decChar2Float(testDecChar_typicalHPS),  413.16265));
  fprintf(stderr,"\n");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(testChecksumCalc)

BOOST_AUTO_TEST_CASE(checksum_correct) {
  BOOST_CHECK_EQUAL(testNeo6m.testChecksum(GPGSV),0);
}

BOOST_AUTO_TEST_CASE(checksum_wrong) {
  BOOST_CHECK_EQUAL(testNeo6m.testChecksum(GPGSA_Error),-1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(testChecksumCalc)

BOOST_AUTO_TEST_CASE(parseSentance) {
fprintf(stderr,"Nmea sent: %.*s\n",(int)sizeof(GPGLL),GPGLL);
testNeo6m.testChecksum(GPGLL);
fprintf(stderr,"Nmea sent: %.*s\n",(int)sizeof(GPGLL),GPGLL);
fprintf(stderr,"\n");
parsedNmeaSent sentOut;
memset(&sentOut,0,sizeof(parsedNmeaSent));
BOOST_CHECK(testNeo6m.parseNmeaStr(GPGLL, sizeof(GPGLL), sentOut)>=0);
int i = 0;
while((sentOut[i][0] != '\0')&(i<=NMEA_MAX_DATA_ARRAY_SIZE)){
  fprintf(stderr," [%i] -> %.*s\n",i,(int)sentOut[i].size(), sentOut[i].data());
  i++;
}

}


BOOST_AUTO_TEST_SUITE_END()
