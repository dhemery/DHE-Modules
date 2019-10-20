cmake_minimum_required(VERSION 3.15)

add_library(stage OBJECT)
target_include_directories(stage PUBLIC .)
target_sources(stage
        PRIVATE
        BoosterStage.cpp
        BoosterStagePanel.cpp
        Hostage.cpp
        HostageMachine.cpp
        HostagePanel.cpp
        Stage.cpp
        StageMachine.cpp
        StagePanel.cpp
        Upstage.cpp
        UpstagePanel.cpp
        )
target_link_libraries(stage
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
set_target_properties(stage
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )
