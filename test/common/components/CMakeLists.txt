add_executable(component_tests
        LatchTest.cpp
        OneShotPhaseAccumulatorTest.cpp
        )
target_link_libraries(component_tests
        PRIVATE
        components
        gtest_main
        )

gtest_add_tests(TARGET component_tests)
