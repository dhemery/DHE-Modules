add_executable(curve_sequencer_tests
        GenerateStageTest.cpp
        SequenceTest.cpp
        StepExecutorTest.cpp
        SustainStageTest.cpp
        )
target_include_directories(curve_sequencer_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(curve_sequencer_tests
        PRIVATE
        gmock_main
        )
gtest_add_tests(TARGET curve_sequencer_tests)
