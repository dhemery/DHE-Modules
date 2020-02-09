add_library(controls INTERFACE)

target_sources(controls INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/CommonInputs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CurvatureInputs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/DurationInputs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/LevelInputs.h
        )

target_include_directories(controls INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

target_link_libraries(controls
        INTERFACE
        dhe::components
        )

add_library(dhe::controls ALIAS controls)
