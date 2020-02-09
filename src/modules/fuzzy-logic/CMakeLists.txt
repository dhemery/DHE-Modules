add_library(fuzzy_logic INTERFACE)

target_sources(fuzzy_logic INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/FuzzyLogicControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/FuzzyLogicH.h
        ${CMAKE_CURRENT_SOURCE_DIR}/FuzzyLogicHPanel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/FuzzyLogicPanel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/FuzzyLogicZ.h
        ${CMAKE_CURRENT_SOURCE_DIR}/FuzzyLogicZPanel.h
        )

target_include_directories(fuzzy_logic INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

target_link_libraries(fuzzy_logic
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )

add_library(dhe::fuzzy_logic ALIAS fuzzy_logic)
