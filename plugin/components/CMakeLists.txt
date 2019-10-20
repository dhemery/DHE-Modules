cmake_minimum_required(VERSION 3.15)

add_library(components INTERFACE)
target_include_directories(components INTERFACE .)

add_executable(component_tests)
target_sources(component_tests
        PRIVATE
        test/LatchTest.cpp
        test/OneShotPhaseAccumulatorTest.cpp
        )
target_link_libraries(component_tests
        PRIVATE
        components
        gtest_main
        )
set_target_properties(component_tests
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )

gtest_discover_tests(component_tests)
