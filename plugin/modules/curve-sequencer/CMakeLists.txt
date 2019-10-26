add_library(curve_sequencer
        CurveSequencer.cpp
        )
target_include_directories(curve_sequencer PUBLIC .)
target_link_libraries(curve_sequencer
        PUBLIC
        controls
        config
        widgets
        Rack::SDK
        )

add_executable(curve_sequencer_tests
        test/GenerateStageTest.cpp
        test/SequenceTest.cpp
        test/StepExecutorTest.cpp
        test/SustainStageTest.cpp
        )
target_link_libraries(curve_sequencer_tests
        PRIVATE
        curve_sequencer
        gmock_main
        )
gtest_add_tests(TARGET curve_sequencer_tests)
