#include <boost/test/unit_test.hpp>

#include <Eigen/Geometry>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/shared_ptr.hpp>

#include <boost_serialization/EigenTypes.hpp>
#include <boost_serialization/BaseTypes.hpp>
#include <boost_serialization/BoostTypes.hpp>

using namespace boost::serialization;

BOOST_AUTO_TEST_CASE(eigen_quaternion_serialization_test)
{
    Eigen::Quaterniond rot = Eigen::AngleAxisd(0.45 * M_PI, Eigen::Vector3d::UnitZ()) *
                            Eigen::AngleAxisd(0.5 * M_PI, Eigen::Vector3d::UnitY()) *
                            Eigen::AngleAxisd(0.27 * M_PI, Eigen::Vector3d::UnitX());

    std::stringstream stream;
    boost::archive::binary_oarchive oa(stream);
    oa << rot;

    // deserialize from string stream
    boost::archive::binary_iarchive ia(stream);
    Eigen::Quaterniond rot_2;
    ia >> rot_2;

    BOOST_CHECK(rot.matrix() == rot_2.matrix());
}

BOOST_AUTO_TEST_CASE(eigen_matrix_serialization_test)
{
    Eigen::Matrix3d m = 0.2 * Eigen::Matrix3d::Identity();

    std::stringstream stream;
    boost::archive::binary_oarchive oa(stream);
    oa << m;

    // deserialize from string stream
    boost::archive::binary_iarchive ia(stream);
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
    boost::archive::binary_oarchive oa(stream);
    oa << transform_o;

    // deserialize from string stream
    boost::archive::binary_iarchive ia(stream);
    base::Transform3d transform_i;
    ia >> transform_i;

    BOOST_CHECK_EQUAL(transform_o.matrix().isApprox(transform_i.matrix()), true); 
}

BOOST_AUTO_TEST_CASE(boost_shared_ptr_serialization_test)
{
    boost::shared_ptr<Eigen::Quaterniond> rot_o(new Eigen::Quaterniond());

    std::stringstream stream;
    boost::archive::binary_oarchive oa(stream);
    oa << rot_o;

    // deserialize from string stream
    boost::archive::binary_iarchive *ia = new boost::archive::binary_iarchive(stream);
    boost::shared_ptr<Eigen::Quaterniond> rot_i;
    (*ia) >> rot_i;

    BOOST_CHECK_EQUAL(rot_o.use_count(), 1);
    // THIS IS BUG IN BOOST
    // the counter is 2 or more after the serialization
    // probably fixed up version 1.57
    BOOST_CHECK_EQUAL(rot_i.use_count(), 2); 
    delete ia;
    BOOST_CHECK_EQUAL(rot_i.use_count(), 1);       
}

BOOST_AUTO_TEST_CASE(boost_multi_array_serialization_test)
{
    boost::multi_array<double, 2> array_o;
    array_o.resize(boost::extents[13][7]);

    for (unsigned int x = 0; x < array_o.shape()[0]; ++x)
    {
        for (unsigned int y = 0; y < array_o.shape()[1]; ++y)
        {
            double cell_value = rand();
            array_o[x][y] = cell_value;
        }
    }    

    std::stringstream stream;
    boost::archive::binary_oarchive oa(stream);
    oa << array_o; 

    boost::archive::binary_iarchive ia(stream);
    boost::multi_array<double, 2> array_i;
    ia >> array_i;    

    BOOST_CHECK_EQUAL(array_o.shape()[0], array_i.shape()[0]);
    BOOST_CHECK_EQUAL(array_o.shape()[1], array_i.shape()[1]); 
    
    BOOST_CHECK(array_o == array_i);
}