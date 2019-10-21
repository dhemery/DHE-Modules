add_library(curve_sequencer_processor INTERFACE)
target_include_directories(curve_sequencer_processor INTERFACE processor)
target_link_libraries(curve_sequencer_processor
        INTERFACE
        components
        )

add_library(curve_sequencer
        CurveSequencer.cpp
        )
target_include_directories(curve_sequencer PUBLIC .)
target_link_libraries(curve_sequencer
        PUBLIC
        curve_sequencer_processor
        controls
        config
        widgets
        )

add_executable(curve_sequencer_tests
        test/GenerateStageTest.cpp
        test/SequenceTest.cpp
        test/StepExecutorTest.cpp
        test/SustainStageTest.cpp
        )
target_link_libraries(curve_sequencer_tests
        PRIVATE
        curve_sequencer_processor
        gmock_main
        )

gtest_discover_tests(curve_sequencer_tests)
