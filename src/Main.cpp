#include <iostream>
#include <boost_serialization/Dummy.hpp>

int main(int argc, char** argv)
{
    boost_serialization::DummyClass dummyClass;
    dummyClass.welcome();

    return 0;
}
