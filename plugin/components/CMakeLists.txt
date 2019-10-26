add_library(components INTERFACE)
target_compile_features(components INTERFACE cxx_std_11)
target_include_directories(components INTERFACE .)

add_executable(component_tests
        test/LatchTest.cpp
        test/OneShotPhaseAccumulatorTest.cpp
        )
target_link_libraries(component_tests
        PRIVATE
        components
        gtest_main
        )

gtest_add_tests(TARGET component_tests)
