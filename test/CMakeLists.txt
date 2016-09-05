find_package(Boost COMPONENTS serialization)

set(deps_pkg_config base-types)

rock_testsuite(test_serialization suite.cpp
   test_serialization.cpp
   DEPS_PKGCONFIG ${deps_pkg_config}
   DEPS_PLAIN
     Boost_SERIALIZATION
)
