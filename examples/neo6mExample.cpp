#include <neo6m.h>


// define an inherited class which implements the has sample method
class neo6mTester: public NEO6M {
public:
  virtual void hasMeasurementCB(neo6mMeasurment thisMesurement) {
    fprintf(stderr, "Doing nothing for now!\n");
  };
};


//BOOST_WARN(1==1); //warn and continue
//BOOST_CHECK(1>0); //fail but continue
//BOOST_REQUIRE(1!=2); //fail and terminate

int main(){
//instantiate neo6m
neo6mTester testNeo6m;


}
