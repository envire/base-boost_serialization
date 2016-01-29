#include <boost/test/unit_test.hpp>

#include <boost_serialization/EigenTypes.hpp>

using namespace boost::serialization;

BOOST_AUTO_TEST_CASE(test_eigen_serialization)
{
    Eigen::Affine3d trans = Eigen::Affine3d::Identity();

}
