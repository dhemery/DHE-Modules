add_executable(stage_tests)
target_include_directories(stage_tests
        PRIVATE
        ${PROJECT_SOURCE_DIR}/plugin/modules
        )
target_sources(stage_tests
        PRIVATE
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

