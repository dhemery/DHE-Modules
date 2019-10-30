add_library(stage STATIC)
add_library(dhe::stage ALIAS stage)
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
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )