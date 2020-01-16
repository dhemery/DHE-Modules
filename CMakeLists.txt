cmake_minimum_required(VERSION 3.12.4)
project(DHE-Modules VERSION 1.6.0 LANGUAGES CXX)

enable_testing()

set(RACK_STAGING_DIR rack-user-dir)
set(RACK_PLUGINS_DIR ${RACK_STAGING_DIR}/plugins-v1/${PROJECT_NAME})

add_subdirectory(dependencies)
add_subdirectory(src)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(tests)
endif()

install(DIRECTORY svg
        DESTINATION ${RACK_PLUGINS_DIR}
        )
install(DIRECTORY presets
        DESTINATION ${RACK_PLUGINS_DIR}
        )
install(FILES LICENSE.txt plugin.json
        DESTINATION ${RACK_PLUGINS_DIR}
        )
