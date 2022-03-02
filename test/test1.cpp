#define BOOST_TEST_MODULE testModuleName
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

//BOOST_WARN(1==1); //warn and continue
//BOOST_CHECK(1>0); //fail but continue
//BOOST_REQUIRE(1!=2); //fail and terminate

#include <sqr.h>
BOOST_AUTO_TEST_CASE(PassTest)
{
    BOOST_CHECK_EQUAL(4, sqr(2));
}

BOOST_AUTO_TEST_CASE(FailTest)
{
    BOOST_CHECK_EQUAL(4, sqr(3));
}