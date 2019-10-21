cmake_minimum_required(VERSION 3.15)

add_library(components INTERFACE)
target_compile_features(components INTERFACE cxx_std_11)
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

gtest_discover_tests(component_tests)
