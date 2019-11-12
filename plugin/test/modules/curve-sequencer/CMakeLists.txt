add_executable(curve_sequencer_tests)
target_include_directories(curve_sequencer_tests
        PRIVATE
        ${PROJECT_SOURCE_DIR}/plugin/modules
        )
target_sources(curve_sequencer_tests
        PRIVATE
        AdvancingTest.cpp
        CurveSequencerTest.cpp
        CurveSequencerControlsTest.cpp
        GenerateStageTest.cpp
        IdleTest.cpp
        StepSelectorTest.cpp
        SustainStageTest.cpp
        )
target_link_libraries(curve_sequencer_tests
        PRIVATE
        dhe::components
        dhe::controls
        dhe::curve_sequencer
        Rack::SDK
        gmock_main
        )
gtest_add_tests(TARGET curve_sequencer_tests)

