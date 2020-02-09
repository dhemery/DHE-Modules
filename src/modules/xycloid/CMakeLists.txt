add_library(xycloid INTERFACE)

target_sources(xycloid INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/RatioKnobParamQuantity.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SpeedKnobParamQuantity.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Xycloid.h
        ${CMAKE_CURRENT_SOURCE_DIR}/XycloidControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/XycloidPanel.h
        )

target_include_directories(xycloid INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

target_link_libraries(xycloid
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )

add_library(dhe::xycloid ALIAS xycloid)
