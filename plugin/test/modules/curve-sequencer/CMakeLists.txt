add_executable(curve_sequencer_tests)
get_target_property(DHE_MODULE_INCLUDES plugin INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(curve_sequencer_tests
        PRIVATE
        ${DHE_MODULE_INCLUDES}
        )
target_sources(curve_sequencer_tests
        PRIVATE
        GenerateStageTest.cpp
        SequenceTest.cpp
        StepExecutorTest.cpp
        SustainStageTest.cpp
        )
target_link_libraries(curve_sequencer_tests
        PRIVATE
        components
        controls
        curve_sequencer
        gmock_main
        )
gtest_add_tests(TARGET curve_sequencer_tests)
