#pragma once

#include <boost/serialization/version.hpp>

/** Specify the current version number for a templated class.
 * The boost macro BOOST_CLASS_VERSION doesn't work for templated classes.
 * Version numbers limited to 8 bits !!!
 */
#define BOOST_TEMPLATED_CLASS_VERSION(T, N)                            \
namespace boost {                                                      \
namespace serialization {                                              \
template<>                                                             \
template<typename Type>                                                \
struct version<T<Type> >                                               \
{                                                                      \
    typedef mpl::int_<N> type;                                         \
    typedef mpl::integral_c_tag tag;                                   \
    BOOST_STATIC_CONSTANT(int, value = version::type::value);          \
    BOOST_MPL_ASSERT((                                                 \
        boost::mpl::less<                                              \
            boost::mpl::int_<N>,                                       \
            boost::mpl::int_<256>                                      \
        >                                                              \
    ));                                                                \
};                                                                     \
}                                                                      \
}
