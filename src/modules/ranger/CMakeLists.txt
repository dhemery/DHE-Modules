add_library(ranger INTERFACE)

target_sources(ranger INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/Ranger.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RangerControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RangerPanel.h
        )

target_include_directories(ranger INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

target_link_libraries(ranger
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )

add_library(dhe::ranger ALIAS ranger)
