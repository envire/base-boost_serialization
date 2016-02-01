#include <boost/test/unit_test.hpp>

#include <Eigen/Geometry>

#include <boost/archive/polymorphic_text_iarchive.hpp>
#include <boost/archive/polymorphic_text_oarchive.hpp>
#include <boost/archive/polymorphic_binary_iarchive.hpp>
#include <boost/archive/polymorphic_binary_oarchive.hpp>

#include <boost/serialization/shared_ptr.hpp>

#include <boost_serialization/EigenTypes.hpp>
#include <boost_serialization/BaseTypes.hpp>

using namespace boost::serialization;

BOOST_AUTO_TEST_CASE(eigen_quaternion_serialization_test)
{
    Eigen::Quaterniond rot = Eigen::AngleAxisd(0.45 * M_PI, Eigen::Vector3d::UnitZ()) *
                            Eigen::AngleAxisd(0.5 * M_PI, Eigen::Vector3d::UnitY()) *
                            Eigen::AngleAxisd(0.27 * M_PI, Eigen::Vector3d::UnitX());

    std::stringstream stream;
    boost::archive::polymorphic_binary_oarchive oa(stream);
    oa << rot;

    // deserialize from string stream
    boost::archive::polymorphic_binary_iarchive ia(stream);
    Eigen::Quaterniond rot_2;
    ia >> rot_2;

    BOOST_CHECK(rot.matrix() == rot_2.matrix());
}

BOOST_AUTO_TEST_CASE(eigen_matrix_serialization_test)
{
    Eigen::Matrix3d m = 0.2 * Eigen::Matrix3d::Identity();

    std::stringstream stream;
    boost::archive::polymorphic_binary_oarchive oa(stream);
    oa << m;

    // deserialize from string stream
    boost::archive::polymorphic_binary_iarchive ia(stream);
    Eigen::MatrixXd m2;
    ia >> m2;

    BOOST_CHECK(m.rows() == m2.rows());
    BOOST_CHECK(m.cols() == m2.cols());
    BOOST_CHECK(m == m2);
}

BOOST_AUTO_TEST_CASE(eigen_transform_serialization_test)
{
    base::Transform3d transform_o(2.2 * Eigen::Matrix3d::Identity());

    std::stringstream stream;
    boost::archive::polymorphic_binary_oarchive oa(stream);
    oa << transform_o;

    // deserialize from string stream
    boost::archive::polymorphic_binary_iarchive ia(stream);
    base::Transform3d transform_i;
    ia >> transform_i;

    BOOST_CHECK_EQUAL(transform_o.matrix().isApprox(transform_i.matrix()), true); 
}

BOOST_AUTO_TEST_CASE(boost_shared_ptr_serialization_test)
{
    boost::shared_ptr<Eigen::Quaterniond> rot_o(new Eigen::Quaterniond());

    std::stringstream stream;
    boost::archive::polymorphic_binary_oarchive oa(stream);
    oa << rot_o;

    // deserialize from string stream
    boost::archive::polymorphic_binary_iarchive ia(stream);
    boost::shared_ptr<Eigen::Quaterniond> rot_i;
    ia >> rot_i;

    BOOST_CHECK_EQUAL(rot_o.use_count(), 1);
    // THIS IS BUG IN BOOST
    // the counter is 2 or more after the serialization
    // probably fixed up version 1.57
    BOOST_CHECK_EQUAL(rot_i.use_count(), 1);        
}