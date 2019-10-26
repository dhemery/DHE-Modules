add_executable(control_tests)
target_include_directories(control_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_sources(control_tests
        PRIVATE
        DurationInputTest.cpp
        LevelInputTest.cpp
        )
target_link_libraries(control_tests
        PRIVATE
        controls
        Rack::SDK
        gtest_main
        )
target_link_options(control_tests PRIVATE -flto)
gtest_add_tests(TARGET control_tests)
