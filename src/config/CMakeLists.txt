add_library(config INTERFACE)

target_sources(config INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/CommonConfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CurvatureConfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/DurationConfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/LevelConfig.h
        )

target_include_directories(config INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

target_link_libraries(config
        INTERFACE
        dhe::components
        dhe::controls
        Rack::SDK
        )

add_library(dhe::config ALIAS config)
