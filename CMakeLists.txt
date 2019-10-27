cmake_minimum_required(VERSION 3.12.4)
project(DHE-Modules VERSION 1.6.0 LANGUAGES CXX)
enable_testing()

# So that gtest doesn't install itself to the usual system dirs
set(CMAKE_INSTALL_PREFIX dale_install)

set(PLUGIN_INSTALL_DIR ${PROJECT_SOURCE_DIR}/.install/plugins-v1/${PROJECT_NAME})

add_subdirectory(external)
add_subdirectory(plugin)

install(DIRECTORY svg
        DESTINATION ${PLUGIN_INSTALL_DIR}
        )
install(FILES LICENSE.txt plugin.json
        DESTINATION ${PLUGIN_INSTALL_DIR}
        )
