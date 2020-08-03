add_library(stage INTERFACE)

target_sources(stage INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/BoosterStageControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/BoosterStageModule.h
        ${CMAKE_CURRENT_SOURCE_DIR}/BoosterStagePanel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/DeferMode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Event.h
        ${CMAKE_CURRENT_SOURCE_DIR}/GenerateMode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/HoldMode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/HostageControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/HostageEngine.h
        ${CMAKE_CURRENT_SOURCE_DIR}/HostageModule.h
        ${CMAKE_CURRENT_SOURCE_DIR}/HostagePanel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/IdleMode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/InputMode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/LevelMode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Mode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/StageControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/StageEngine.h
        ${CMAKE_CURRENT_SOURCE_DIR}/StageModule.h
        ${CMAKE_CURRENT_SOURCE_DIR}/StagePanel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SustainMode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UpstageControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UpstageEngine.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UpstageModule.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UpstagePanel.h
        )

target_include_directories(stage INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

target_link_libraries(stage
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )

add_library(dhe::stage ALIAS stage)
