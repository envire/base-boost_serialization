find_package(Boost COMPONENTS serialization)

set(deps_pkg_config base-types numeric)

set(headers EigenTypes.hpp
            BoostTypes.hpp
            BaseTypes.hpp
            BaseNumeric.hpp
            DynamicSizeSerialization.hpp
            ClassVersion.hpp)

rock_library(boost_serialization
    HEADERS ${headers}
    DEPS_CMAKE Boost
    DEPS_PKGCONFIG ${deps_pkg_config})

rock_install_headers(${headers})

