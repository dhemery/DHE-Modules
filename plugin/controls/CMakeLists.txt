cmake_minimum_required(VERSION 3.15)

add_library(controls OBJECT)
target_include_directories(controls PUBLIC .)
target_sources(controls
        PRIVATE
        CommonInputs.cpp
        DurationInputs.cpp
        )
target_link_libraries(controls
        PUBLIC
        components
        racksdk
        )
set_target_properties(controls
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )

add_executable(control_tests)
target_sources(control_tests
        PRIVATE
        test/DurationInputTest.cpp
        test/LevelInputTest.cpp
        )
target_link_libraries(control_tests
        PRIVATE
        controls
        gtest_main
        )
set_target_properties(control_tests
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )

gtest_discover_tests(control_tests)