add_library(components INTERFACE)

target_sources(components INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/Latch.h
        ${CMAKE_CURRENT_SOURCE_DIR}/PhaseRotor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/PhaseTimer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Range.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Sigmoid.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Taper.h
        )

target_include_directories(components INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

add_library(dhe::components ALIAS components)
