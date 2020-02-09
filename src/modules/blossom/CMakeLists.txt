add_library(blossom INTERFACE)

target_sources(widgets INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/Blossom.h
        ${CMAKE_CURRENT_SOURCE_DIR}/BlossomControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/BlossomPanel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RatioKnobParamQuantity.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SpeedKnobParamQuantity.h
        )

target_include_directories(widgets INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

target_link_libraries(blossom
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )

add_library(dhe::blossom ALIAS blossom)
