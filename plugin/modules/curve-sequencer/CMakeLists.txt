cmake_minimum_required(VERSION 3.15)

add_library(curve_sequencer_processor INTERFACE)
target_include_directories(curve_sequencer_processor INTERFACE processor)
target_link_libraries(curve_sequencer_processor
        INTERFACE
        components
        )

add_library(curve_sequencer)
target_include_directories(curve_sequencer PUBLIC .)
target_sources(curve_sequencer
        PRIVATE
        CurveSequencer.cpp
        )
target_link_libraries(curve_sequencer
        PUBLIC
        curve_sequencer_processor
        controls
        config
        widgets
        )

add_executable(curve_sequencer_tests)
target_sources(curve_sequencer_tests
        PRIVATE
        test/GenerateStageTest.cpp
        test/SequenceTest.cpp
        test/StepExecutorTest.cpp
        test/SustainStageTest.cpp
        )
target_compile_options(curve_sequencer_tests
        PRIVATE
        # Without optimization level 2 or higher,
        # gtest fails on one of my computers
        -O2
        )
target_link_libraries(curve_sequencer_tests
        PRIVATE
        curve_sequencer_processor
        gmock_main
        )

gtest_discover_tests(curve_sequencer_tests)
