add_library(widgets INTERFACE)

target_sources(widgets INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/ControlWidgets.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Dimensions.h
        ${CMAKE_CURRENT_SOURCE_DIR}/PanelAssets.h
        ${CMAKE_CURRENT_SOURCE_DIR}/PickList.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Screws.h
        )

target_include_directories(widgets INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

add_library(dhe::widgets ALIAS widgets)
