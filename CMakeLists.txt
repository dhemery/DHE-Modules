cmake_minimum_required(VERSION 3.12.4)
project(DHE-Modules VERSION 1.6.0 LANGUAGES CXX)
enable_testing()

add_subdirectory(external)
add_subdirectory(plugin)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/rack)
set(PLUGINS_INSTALL_DIR plugins-v1)
set(PLUGIN_INSTALL_DIR plugins-v1/${PROJECT_NAME})

install(TARGETS plugin
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${PLUGIN_INSTALL_DIR}
        )
install(DIRECTORY ${PROJECT_SOURCE_DIR}/svg
        DESTINATION ${PLUGIN_INSTALL_DIR}
        )
install(FILES
        ${PROJECT_SOURCE_DIR}/LICENSE.txt
        ${PROJECT_SOURCE_DIR}/plugin.json
        DESTINATION ${PLUGIN_INSTALL_DIR}
        )
