add_library(gator INTERFACE)

target_sources(gator INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/Gator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/GatorControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/GatorPanel.h
        )

target_include_directories(gator INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

target_link_libraries(gator
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )

add_library(dhe::gator ALIAS gator)
