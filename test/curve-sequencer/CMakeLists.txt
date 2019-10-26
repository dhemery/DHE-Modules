add_executable(curve_sequencer_tests
        GenerateStageTest.cpp
        SequenceTest.cpp
        StepExecutorTest.cpp
        SustainStageTest.cpp
        )
target_link_libraries(curve_sequencer_tests
        PRIVATE
        curve_sequencer
        gmock_main
        )
gtest_add_tests(TARGET curve_sequencer_tests)
