add_executable(stage_tests)
target_include_directories(stage_tests
        PRIVATE
        ${PROJECT_SOURCE_DIR}/plugin
        )
target_sources(stage_tests
        PRIVATE
        DeferModeTest.cpp
        GenerateModeTest.cpp
        HoldModeTest.cpp
        HostageEngineTest.cpp
        IdleModeTest.cpp
        InputModeTest.cpp
        LevelModeTest.cpp
        StageEngineTest.cpp
        )
target_link_libraries(stage_tests
        PRIVATE
        dhe::components
        dhe::controls
        dhe::stage
        Rack::SDK
        gmock_main
        )
gtest_add_tests(TARGET stage_tests)

