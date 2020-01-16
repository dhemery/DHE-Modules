add_executable(control_tests)
target_include_directories(control_tests
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        )
target_sources(control_tests
        PRIVATE
        DurationInputTest.cpp
        LevelInputTest.cpp
        )
target_link_libraries(control_tests
        PRIVATE
        dhe::controls
        Rack::SDK
        gtest_main
        )
gtest_add_tests(TARGET control_tests)
