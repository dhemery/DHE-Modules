add_library(cubic INTERFACE)

target_sources(cubic INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/Cubic.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CubicControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CubicPanel.h
        )

target_include_directories(cubic INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

target_link_libraries(cubic
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )

add_library(dhe::cubic ALIAS cubic)
