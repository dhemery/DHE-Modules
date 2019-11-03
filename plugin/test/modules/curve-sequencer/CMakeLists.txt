add_executable(curve_sequencer_tests)
target_include_directories(curve_sequencer_tests
        PRIVATE
        ${PROJECT_SOURCE_DIR}/plugin/modules
        )
target_sources(curve_sequencer_tests
        PRIVATE
        AdvancingModeTest.cpp
        CurveSequencerTest.cpp
        GeneratingModeTest.cpp
        IdleModeTest.cpp
        PauseModeTest.cpp
        SustainingModeTest.cpp
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

