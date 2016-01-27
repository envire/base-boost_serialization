rock_library(boost_serialization
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(boost_serialization_bin Main.cpp
    DEPS boost_serialization)

