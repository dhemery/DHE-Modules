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

        plugin/modules/blossom/Blossom.cpp
        plugin/modules/blossom/BlossomPanel.cpp

        plugin/modules/cubic/Cubic.cpp
        plugin/modules/cubic/CubicPanel.cpp

        plugin/modules/curve-sequencer/CurveSequencer.cpp

        plugin/modules/func/Func.cpp
        plugin/modules/func/FuncPanel.cpp
        plugin/modules/func/Func6.cpp
        plugin/modules/func/Func6Panel.cpp
        plugin/modules/func/FuncChannel.cpp

        plugin/modules/fuzzy-logic/FuzzyLogicH.cpp
        plugin/modules/fuzzy-logic/FuzzyLogicHPanel.cpp
        plugin/modules/fuzzy-logic/FuzzyLogicZ.cpp
        plugin/modules/fuzzy-logic/FuzzyLogicZPanel.cpp

        plugin/modules/gator/Gator.cpp
        plugin/modules/gator/GatorPanel.cpp

        plugin/modules/ranger/Ranger.cpp
        plugin/modules/ranger/RangerPanel.cpp

        plugin/modules/stage/BoosterStage.cpp
        plugin/modules/stage/BoosterStagePanel.cpp
        plugin/modules/stage/Hostage.cpp
        plugin/modules/stage/HostageMachine.cpp
        plugin/modules/stage/HostagePanel.cpp
        plugin/modules/stage/Stage.cpp
        plugin/modules/stage/StageMachine.cpp
        plugin/modules/stage/StagePanel.cpp
        plugin/modules/stage/Upstage.cpp
        plugin/modules/stage/UpstagePanel.cpp

        plugin/modules/swave/Swave.cpp
        plugin/modules/swave/SwavePanel.cpp

        plugin/modules/tapers/Tapers.cpp
        plugin/modules/tapers/TapersPanel.cpp

        plugin/modules/xycloid/Xycloid.cpp
        plugin/modules/xycloid/XycloidPanel.cpp

        )
target_link_libraries(modules
        PUBLIC
        components
        controls
        config
        curve_sequencer
        racksdk
        )

add_library(plugin SHARED
        plugin/PluginInitializer.cpp
        )
target_link_libraries(plugin
        PRIVATE
        components
        controls
        modules
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

add_executable(componenttests)
target_sources(componenttests
        PRIVATE
        plugin/components/LatchTests.cpp
        plugin/components/OneShotPhaseAccumulatorTests.cpp
        )
target_link_libraries(componenttests
        PRIVATE
        components
        gmock_main
        )
gtest_discover_tests(componenttests)

add_executable(controltests)
target_sources(controltests
        PRIVATE
        plugin/controls/DurationInputTests.cpp
        plugin/controls/LevelInputTests.cpp
        )
target_link_libraries(controltests
        PRIVATE
        controls
        gmock_main
        )
gtest_discover_tests(controltests)

add_executable(moduletests)
target_sources(moduletests
        PRIVATE
        plugin/modules/curve-sequencer/GenerateStageTests.cpp
        plugin/modules/curve-sequencer/SequenceTests.cpp
        plugin/modules/curve-sequencer/StepExecutorTests.cpp
        plugin/modules/curve-sequencer/SustainStageTests.cpp
        )
target_link_libraries(moduletests
        PRIVATE
        components
        controls
        gmock_main
        )
gtest_discover_tests(moduletests)

set_target_properties(
        componenttests
        controls
        controltests
        config
        modules
        moduletests
        plugin
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        PREFIX ""
)

gtest_discover_tests(componenttests)
enable_testing()
