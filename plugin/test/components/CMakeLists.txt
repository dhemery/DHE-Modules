add_executable(component_tests)
target_sources(component_tests
        PRIVATE
        LatchTest.cpp
        PhaseTimerTest.cpp
        )
target_link_libraries(component_tests
        PRIVATE
        dhe::components
        gtest_main
        )
target_include_directories(component_tests
        PRIVATE
        ${PROJECT_SOURCE_DIR}/plugin
        )

gtest_add_tests(TARGET component_tests)
