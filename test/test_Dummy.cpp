#include <boost/test/unit_test.hpp>
#include <boost_serialization/Dummy.hpp>

using namespace boost_serialization;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    boost_serialization::DummyClass dummy;
    dummy.welcome();
}
