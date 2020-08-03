add_library(swave INTERFACE)

target_sources(swave INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/Swave.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SwaveControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SwavePanel.h
        )

target_include_directories(swave INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

target_link_libraries(swave
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )

add_library(dhe::swave ALIAS swave)
