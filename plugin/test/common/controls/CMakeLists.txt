add_executable(control_tests)
target_sources(control_tests
        PRIVATE
        DurationInputTest.cpp
        LevelInputTest.cpp
        )
target_link_libraries(control_tests
        PRIVATE
        controls
        gtest_main
        )
gtest_add_tests(TARGET control_tests)
