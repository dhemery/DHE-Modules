add_library(widgets INTERFACE)
target_sources(widgets INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/ControlWidgets.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Dimensions.h
        ${CMAKE_CURRENT_SOURCE_DIR}/MenuWidgets.h
        ${CMAKE_CURRENT_SOURCE_DIR}/PanelAssets.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Screws.h
        )
target_include_directories(widgets INTERFACE .)

add_library(dhe::widgets ALIAS widgets)
