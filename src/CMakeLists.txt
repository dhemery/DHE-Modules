add_compile_options(-Wall -Wextra -Wno-unused-parameter -g -O3 -std=c++11 -fPIC)

if (WIN32)
    add_compile_definitions(ARCH_WIN _USE_MATH_DEFINES)
endif ()

if (APPLE)
    add_compile_definitions(ARCH_MAC)
    add_compile_options(-mmacosx-version-min=10.7 -stdlib=libc++)
else ()
    add_compile_options(-Wsuggest-override)
endif ()

if (UNIX AND NOT APPLE)
    add_compile_definitions(ARCH_LIN)
endif ()

add_library(components INTERFACE)
target_include_directories(components INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_library(controls OBJECT)
target_include_directories(controls PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_sources(controls
        PRIVATE
        controls/CommonInputs.cpp
        controls/DurationInputs.cpp
        )
target_link_libraries(controls
        PUBLIC
        components
        Rack::SDK
        )

add_library(config OBJECT)
target_include_directories(config PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_sources(config
        PRIVATE
        config/CurvatureConfig.cpp
        config/DurationConfig.cpp
        config/LevelConfig.cpp
        )
target_link_libraries(config
        PUBLIC
        components
        controls
        Rack::SDK
        )

add_library(modules OBJECT)
target_include_directories(modules PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_sources(modules
        PRIVATE
        module/blossom/Blossom.cpp
        module/blossom/BlossomPanel.cpp
        module/cubic/Cubic.cpp
        module/cubic/CubicPanel.cpp
        module/curve-sequencer/CurveSequencer.cpp
        module/func/Func.cpp
        module/func/Func6.cpp
        module/func/FuncPanel.cpp
        module/func/Func6Panel.cpp
        module/func/FuncChannel.cpp
        module/func/FuncSteppers.h
        module/fuzzy-logic/FuzzyLogicH.cpp
        module/fuzzy-logic/FuzzyLogicHPanel.cpp
        module/fuzzy-logic/FuzzyLogicZ.cpp
        module/fuzzy-logic/FuzzyLogicZPanel.cpp
        module/gator/Gator.cpp
        module/gator/GatorPanel.cpp
        module/ranger/Ranger.cpp
        module/ranger/RangerPanel.cpp
        module/stage/BoosterStage.cpp
        module/stage/BoosterStagePanel.cpp
        module/stage/Hostage.cpp
        module/stage/HostagePanel.cpp
        module/stage/Stage.cpp
        module/stage/StagePanel.cpp
        module/stage/Upstage.cpp
        module/stage/UpstagePanel.cpp
        module/stage/StageMachine.cpp
        module/stage/HostageMachine.cpp
        module/swave/Swave.cpp
        module/swave/SwavePanel.cpp
        module/tapers/Tapers.cpp
        module/tapers/TapersPanel.cpp
        module/xycloid/Xycloid.cpp
        module/xycloid/XycloidPanel.cpp
        )
target_link_libraries(modules
        PUBLIC
        components
        controls
        config
        Rack::SDK
        )

add_library(plugin SHARED)
target_include_directories(plugin PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_sources(plugin
        PRIVATE
        plugin/PluginInitializer.cpp
        )
target_link_libraries(plugin
        PUBLIC
        components
        controls
        config
        modules
        Rack::SDK
        )
IF (APPLE)
    target_link_options(plugin PUBLIC "LINKER:-undefined,dynamic_lookup")
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
