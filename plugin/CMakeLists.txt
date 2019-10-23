enable_testing()
add_compile_options(-Wall -Wextra -Wno-unused-parameter -g -O3 -std=c++11 -fPIC)

if (APPLE)
    add_compile_definitions(ARCH_MAC)
    add_compile_options(-mmacosx-version-min=10.7 -stdlib=libc++)
endif ()

if (UNIX AND NOT APPLE)
    add_compile_definitions(ARCH_LIN)
    add_compile_options(-Wsuggest-override)
endif ()

if (NOT UNIX)
    set(CMAKE_CXX_COMPILER $ENV{CXX})
    add_compile_definitions(ARCH_WIN _USE_MATH_DEFINES)
    add_compile_options(-Wsuggest-override)
endif ()

add_subdirectory(components)
add_subdirectory(controls)
add_subdirectory(config)
add_subdirectory(widgets)

add_subdirectory(modules/blossom)
add_subdirectory(modules/cubic)
add_subdirectory(modules/curve-sequencer)
add_subdirectory(modules/func)
add_subdirectory(modules/fuzzy-logic)
add_subdirectory(modules/gator)
add_subdirectory(modules/ranger)
add_subdirectory(modules/stage)
add_subdirectory(modules/swave)
add_subdirectory(modules/tapers)
add_subdirectory(modules/xycloid)

add_library(plugin SHARED)
target_sources(plugin
        PRIVATE
        PluginInitializer.cpp
        )
target_link_libraries(plugin
        PRIVATE
        blossom
        cubic
        curve_sequencer
        func
        fuzzy_logic
        gator
        ranger
        stage
        swave
        tapers
        xycloid
        )
IF (NOT UNIX)
    target_link_options(plugin PRIVATE -L${racksdk_SOURCE_DIR} -lRack)
endif ()
IF (APPLE)
    target_link_options(plugin PRIVATE "LINKER:-undefined,dynamic_lookup")
endif ()
set_target_properties(plugin PROPERTIES PREFIX "")

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/rack)
set(PLUGINS_INSTALL_DIR plugins-v1)
set(PLUGIN_INSTALL_DIR plugins-v1/${PROJECT_NAME})

install(TARGETS plugin
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${PLUGIN_INSTALL_DIR}
        )
install(DIRECTORY ../svg DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES ../LICENSE.txt ../plugin.json DESTINATION ${PLUGIN_INSTALL_DIR})
