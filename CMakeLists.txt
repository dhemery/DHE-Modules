cmake_minimum_required(VERSION 3.15)
project(DHE-Modules
        VERSION 1.6.0
        LANGUAGES CXX)

add_library(racksdk INTERFACE)
target_include_directories(racksdk
        INTERFACE
        ../Rack-SDK/include
        ../Rack-SDK/dep/include)
target_compile_features(racksdk
        INTERFACE
        cxx_std_11
        )
target_compile_options(racksdk
        INTERFACE
        -stdlib=libc++
        -march=nocona
        -MMD
        -MP
        -O3
        -ffast-math
        -fno-finite-math-only
        -mmacosx-version-min=10.7
        -Wall
        -Wextra
        -Wno-unused-parameter
        )

add_library(components INTERFACE)
target_sources(components
        INTERFACE
        include/modules/components/Latch.h
        include/modules/components/LoopingPhaseAccumulator.h
        include/modules/components/OneShotPhaseAccumulator.h
        include/modules/components/Range.h
        include/modules/components/Sigmoid.h
        include/modules/components/Taper.h
        )
target_include_directories(components
        INTERFACE
        include
        )
target_compile_definitions(components
        INTERFACE
        ARCH_MAC
        )

add_library(controls INTERFACE)
target_sources(controls
        INTERFACE
        include/modules/controls/CommonInputs.h
        include/modules/controls/CurvatureInputs.h
        include/modules/controls/DurationInputs.h
        include/modules/controls/LevelInputs.h
        src/modules/controls/CommonInputs.cpp
        src/modules/controls/DurationInputs.cpp
        )
target_link_libraries(controls
        INTERFACE
        components
        racksdk
        )

add_library(curve_sequencer INTERFACE)
target_sources(curve_sequencer
        INTERFACE
        include/modules/curve-sequencer/Sequence.h
        include/modules/curve-sequencer/Stages.h
        include/modules/curve-sequencer/StepExecutor.h
        )
target_link_libraries(curve_sequencer
        INTERFACE
        components
        )

add_library(modules OBJECT)
target_sources(modules
        PUBLIC
        include/modules/Blossom.h
        include/modules/BoosterStage.h
        include/modules/Cubic.h
        include/modules/CurveSequencer.h
        include/modules/Func.h
        include/modules/Func6.h
        include/modules/FuzzyLogicH.h
        include/modules/FuzzyLogicZ.h
        include/modules/Gator.h
        include/modules/Hostage.h
        include/modules/Ranger.h
        include/modules/Stage.h
        include/modules/Swave.h
        include/modules/Tapers.h
        include/modules/Upstage.h
        include/modules/Xycloid.h

        include/modules/controls/CommonConfig.h
        include/modules/controls/CurvatureConfig.h
        include/modules/controls/DurationConfig.h
        include/modules/controls/LevelConfig.h

        include/modules/envelopes/HostageMachine.h
        include/modules/envelopes/StageMachine.h

        include/modules/func/FuncChannel.h

        src/modules/Blossom.cpp
        src/modules/BoosterStage.cpp
        src/modules/Cubic.cpp
        src/modules/CurveSequencer.cpp
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

        src/modules/controls/LevelConfig.cpp
        src/modules/controls/CurvatureConfig.cpp
        src/modules/controls/DurationConfig.cpp

        src/modules/envelopes/StageMachine.cpp
        src/modules/envelopes/HostageMachine.cpp

        src/modules/func/FuncChannel.cpp
        )
target_link_libraries(modules
        PUBLIC
        components
        controls
        curve_sequencer
        racksdk
        )

add_library(panels OBJECT)
target_sources(panels
        PUBLIC
        include/panels/BlossomPanel.h
        include/panels/BoosterStagePanel.h
        include/panels/CubicPanel.h
        include/panels/CurveSequencerPanel.h
        include/panels/FuncPanel.h
        include/panels/Func6Panel.h
        include/panels/FuzzyLogicHPanel.h
        include/panels/FuzzyLogicZPanel.h
        include/panels/GatorPanel.h
        include/panels/HostagePanel.h
        include/panels/Panel.h
        include/panels/RangerPanel.h
        include/panels/Sequence.h
        include/panels/StagePanel.h
        include/panels/SwavePanel.h
        include/panels/TapersPanel.h
        include/panels/UpstagePanel.h
        include/panels/XycloidPanel.h

        include/panels/func/FuncSteppers.h

        include/panels/widgets/Buttons.h
        include/panels/widgets/Knobs.h
        include/panels/widgets/Toggles.h

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

add_library(plugin
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
        "LINKER:-undefined,dynamic_lookup"
        )
set_target_properties(plugin
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        PREFIX ""
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
        test/components/LatchTests.cpp
        test/components/OneShotPhaseAccumulatorTests.cpp
        test/controls/DurationInputTests.cpp
        test/controls/LevelInputTests.cpp
        test/curve-sequencer/GenerateStageTests.cpp
        test/curve-sequencer/SequenceTests.cpp
        test/curve-sequencer/StepExecutorTests.cpp
        test/curve-sequencer/SustainStageTests.cpp
        )

target_link_libraries(unittests
        PUBLIC
        components
        controls
        gmock_main
        )
set_target_properties(unittests
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        PREFIX ""
        )

gtest_discover_tests(unittests)
enable_testing()
