cmake_minimum_required(VERSION 3.15)
project(DHE-Modules
        VERSION 1.6.0
        LANGUAGES CXX)

add_compile_options(
        -Wall -Wextra -Wno-unused-parameter
        -g

        # Lin and Win only
        # -Wsuggest-override

        # Mac only (FLAGS and LDFLAGS)
        -stdlib=libc++
        -mmacosx-version-min=10.7
)

add_compile_definitions(
        # ARCH_LIN # Linux Only
        # ARCH_WIN # Windows Only
        ARCH_MAC # Mac Only

        # Win only
        # _USE_MATH_DEFINES
)

add_library(racksdk INTERFACE)
target_include_directories(racksdk
        INTERFACE
        ../Rack-SDK/include
        ../Rack-SDK/dep/include
        )
target_compile_features(racksdk
        INTERFACE
        cxx_std_11
        )

add_library(components INTERFACE)
target_include_directories(components INTERFACE plugin)

add_library(controls OBJECT)
target_include_directories(controls PUBLIC plugin)
target_sources(controls PRIVATE
        plugin/controls/CommonInputs.cpp
        plugin/controls/DurationInputs.cpp
        )
target_link_libraries(controls
        PUBLIC
        components
        racksdk
        )

add_library(config OBJECT)
target_include_directories(config PUBLIC plugin)
target_sources(config
        PRIVATE
        plugin/config/LevelConfig.cpp
        plugin/config/CurvatureConfig.cpp
        plugin/config/DurationConfig.cpp
        )
target_link_libraries(config
        PUBLIC
        components
        racksdk
        )

add_library(curve_sequencer INTERFACE)
target_include_directories(curve_sequencer INTERFACE include)
target_link_libraries(curve_sequencer
        INTERFACE
        components
        )

add_library(modules OBJECT)
target_include_directories(modules PUBLIC include)
target_sources(modules
        PRIVATE
        src/modules/Blossom.cpp
        src/modules/BoosterStage.cpp
        src/modules/Cubic.cpp
        plugin/modules/curve-sequencer/CurveSequencer.cpp
        src/modules/Hostage.cpp
        src/modules/Stage.cpp
        src/modules/Func.cpp
        src/modules/Func6.cpp
        src/modules/FuzzyLogicH.cpp
        src/modules/FuzzyLogicZ.cpp
        src/modules/Gator.cpp
        src/modules/Ranger.cpp
        src/modules/Swave.cpp
        src/modules/Tapers.cpp
        src/modules/Upstage.cpp
        src/modules/Xycloid.cpp

        src/modules/envelopes/StageMachine.cpp
        src/modules/envelopes/HostageMachine.cpp

        src/modules/func/FuncChannel.cpp
        )
target_link_libraries(modules
        PUBLIC
        components
        controls
        config
        curve_sequencer
        racksdk
        )

add_library(panels OBJECT)
target_include_directories(panels PUBLIC include)
target_sources(panels
        PRIVATE
        src/panels/BlossomPanel.cpp
        src/panels/BoosterStagePanel.cpp
        src/panels/CubicPanel.cpp
        src/panels/HostagePanel.cpp
        src/panels/FuncPanel.cpp
        src/panels/Func6Panel.cpp
        src/panels/FuzzyLogicHPanel.cpp
        src/panels/FuzzyLogicZPanel.cpp
        src/panels/GatorPanel.cpp
        src/panels/RangerPanel.cpp
        src/panels/StagePanel.cpp
        src/panels/SwavePanel.cpp
        src/panels/TapersPanel.cpp
        src/panels/UpstagePanel.cpp
        src/panels/XycloidPanel.cpp
        )
target_link_libraries(panels
        PUBLIC
        modules
        racksdk
        )

add_library(plugin SHARED
        src/PluginInitializer.cpp
        )
target_link_libraries(plugin
        PRIVATE
        components
        controls
        modules
        panels
        racksdk
        )
target_link_options(plugin
        PRIVATE

        # Mac only
        "LINKER:-undefined,dynamic_lookup"

        # Win only
        # -L$(RACK_DIR) -lRack
        )

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/rack)
set(PLUGINS_INSTALL_DIR plugins)
set(PLUGIN_INSTALL_DIR plugins/${PROJECT_NAME})

install(TARGETS plugin
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${PLUGIN_INSTALL_DIR})
install(DIRECTORY ../svg DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES ../LICENSE.txt ../plugin.json DESTINATION ${PLUGIN_INSTALL_DIR})

include(FetchContent)
include(GoogleTest)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
)

FetchContent_MakeAvailable(googletest)

add_executable(unittests)

target_sources(unittests
        PRIVATE
        plugin/components/LatchTests.cpp
        plugin/components/OneShotPhaseAccumulatorTests.cpp

        plugin/controls/DurationInputTests.cpp
        plugin/controls/LevelInputTests.cpp

        plugin/modules/curve-sequencer/GenerateStageTests.cpp
        plugin/modules/curve-sequencer/SequenceTests.cpp
        plugin/modules/curve-sequencer/StepExecutorTests.cpp
        plugin/modules/curve-sequencer/SustainStageTests.cpp
        )

target_link_libraries(unittests
        PRIVATE
        components
        controls
        gmock_main
        )

set_target_properties(controls config modules unittests plugin
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        PREFIX ""
        )

gtest_discover_tests(unittests)
enable_testing()
