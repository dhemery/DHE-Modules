add_library(controls
        CommonInputs.cpp
        DurationInputs.cpp
        )
target_include_directories(controls INTERFACE .)
target_link_libraries(controls
        PUBLIC
        components
        racksdk
        )

add_executable(control_tests)
target_sources(control_tests
        PRIVATE
        test/DurationInputTest.cpp
        test/LevelInputTest.cpp
        )
target_link_libraries(control_tests
        PRIVATE
        components
        controls
        gtest_main
        )

gtest_discover_tests(control_tests)
