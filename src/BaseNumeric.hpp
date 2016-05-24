#pragma once

#include <boost/serialization/array.hpp>
#include <boost/serialization/nvp.hpp>

#include <numeric/PlaneFitting.hpp>

namespace boost { namespace serialization
{

    /** Serializes base::TransformWithCovariance */
    template<typename _Archive,typename ScalarType>
    inline void serialize(
        _Archive & ar,
        ::numeric::PlaneFitting<ScalarType>& pf,
        const unsigned int version)
    {
        ar & boost::serialization::make_nvp("x", pf.x);
        ar & boost::serialization::make_nvp("y", pf.y);
        ar & boost::serialization::make_nvp("z", pf.z);
        ar & boost::serialization::make_nvp("xx", pf.xx);
        ar & boost::serialization::make_nvp("yy", pf.yy);
        ar & boost::serialization::make_nvp("xy", pf.xy);
        ar & boost::serialization::make_nvp("xz", pf.xz);
        ar & boost::serialization::make_nvp("yz", pf.yz);
        ar & boost::serialization::make_nvp("zz", pf.zz);
        ar & boost::serialization::make_nvp("n", pf.n);
    }

}}