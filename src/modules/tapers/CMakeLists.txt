add_library(tapers INTERFACE)

target_sources(tapers INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/Tapers.h
        ${CMAKE_CURRENT_SOURCE_DIR}/TapersControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/TapersPanel.h
        )

target_include_directories(tapers INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

target_link_libraries(tapers
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )

add_library(dhe::tapers ALIAS tapers)
