cmake_minimum_required(VERSION 3.15)
project(DHE-Modules
        VERSION 1.6.0
        LANGUAGES CXX)

add_library(plugin)

target_sources(plugin
        PRIVATE
        include/panels/FuncPanel.h
        include/panels/CubicPanel.h
        include/panels/TapersPanel.h
        include/panels/BlossomPanel.h
        include/panels/GatorPanel.h
        include/panels/HostagePanel.h
        include/panels/RangerPanel.h
        include/panels/CurveSequencerPanel.h
        include/panels/BoosterStagePanel.h
        include/panels/FuzzyLogicZPanel.h
        include/panels/Panel.h
        include/panels/XycloidPanel.h
        include/panels/Sequence.h
        include/panels/UpstagePanel.h
        include/panels/Func6Panel.h
        include/panels/FuzzyLogicHPanel.h
        include/panels/StagePanel.h
        include/panels/SwavePanel.h
        include/panels/widgets/Toggles.h
        include/panels/widgets/Buttons.h
        include/panels/widgets/Knobs.h
        include/panels/func/FuncSteppers.h
        include/modules/Tapers.h
        include/modules/Upstage.h
        include/modules/Gator.h
        include/modules/Blossom.h
        include/modules/controls/CurvatureConfig.h
        include/modules/controls/LevelConfig.h
        include/modules/controls/DurationInputs.h
        include/modules/controls/CommonInputs.h
        include/modules/controls/CurvatureInputs.h
        include/modules/controls/LevelInputs.h
        include/modules/controls/DurationConfig.h
        include/modules/controls/CommonConfig.h
        include/modules/components/Range.h
        include/modules/components/LoopingPhaseAccumulator.h
        include/modules/components/OneShotPhaseAccumulator.h
        include/modules/components/Sigmoid.h
        include/modules/components/Latch.h
        include/modules/components/Taper.h
        include/modules/FuzzyLogicH.h
        include/modules/Stage.h
        include/modules/Hostage.h
        include/modules/BoosterStage.h
        include/modules/FuzzyLogicZ.h
        include/modules/curve-sequencer/Stages.h
        include/modules/curve-sequencer/StepExecutor.h
        include/modules/curve-sequencer/Sequence.h
        include/modules/Xycloid.h
        include/modules/Cubic.h
        include/modules/Func.h
        include/modules/Ranger.h
        include/modules/CurveSequencer.h
        include/modules/envelopes/HostageMachine.h
        include/modules/envelopes/StageMachine.h
        include/modules/Func6.h
        include/modules/Swave.h
        include/modules/func/FuncChannel.h
        src/panels/RangerPanel.cpp
        src/panels/StagePanel.cpp
        src/panels/BoosterStagePanel.cpp
        src/panels/CubicPanel.cpp
        src/panels/HostagePanel.cpp
        src/panels/Func6Panel.cpp
        src/panels/FuncPanel.cpp
        src/panels/GatorPanel.cpp
        src/panels/TapersPanel.cpp
        src/panels/FuzzyLogicHPanel.cpp
        src/panels/XycloidPanel.cpp
        src/panels/UpstagePanel.cpp
        src/panels/BlossomPanel.cpp
        src/panels/SwavePanel.cpp
        src/panels/FuzzyLogicZPanel.cpp
        src/PluginInitializer.cpp
        src/modules/Stage.cpp
        src/modules/Hostage.cpp
        src/modules/Func.cpp
        src/modules/FuzzyLogicH.cpp
        src/modules/FuzzyLogicZ.cpp
        src/modules/Swave.cpp
        src/modules/CurveSequencer.cpp
        src/modules/Tapers.cpp
        src/modules/Func6.cpp
        src/modules/controls/LevelConfig.cpp
        src/modules/controls/DurationInputs.cpp
        src/modules/controls/CurvatureConfig.cpp
        src/modules/controls/DurationConfig.cpp
        src/modules/controls/CommonInputs.cpp
        src/modules/Ranger.cpp
        src/modules/BoosterStage.cpp
        src/modules/Xycloid.cpp
        src/modules/Upstage.cpp
        src/modules/Blossom.cpp
        src/modules/Gator.cpp
        src/modules/Cubic.cpp
        src/modules/envelopes/StageMachine.cpp
        src/modules/envelopes/HostageMachine.cpp
        src/modules/func/FuncChannel.cpp
        )

set_target_properties(plugin
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        PREFIX ""
        )
target_compile_options(plugin
        PRIVATE
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
target_compile_definitions(plugin
        PRIVATE
        ARCH_MAC
        )
target_include_directories(plugin PRIVATE include)
target_link_libraries(plugin PRIVATE racksdk)
target_link_options(plugin
        PRIVATE
        "LINKER:-undefined,dynamic_lookup"
        )

add_library(racksdk INTERFACE)
target_include_directories(racksdk
        INTERFACE
        ../../Rack-SDK/include
        ../../Rack-SDK/dep/include)
target_compile_features(racksdk INTERFACE
        cxx_std_11)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/rack)
set(PLUGINS_INSTALL_DIR plugins)
set(PLUGIN_INSTALL_DIR plugins/${PROJECT_NAME})

install(TARGETS plugin
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR}
        ARCHIVE DESTINATION ${PLUGIN_INSTALL_DIR})
install(DIRECTORY ../svg DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES ../LICENSE.txt ../plugin.json DESTINATION ${PLUGIN_INSTALL_DIR})
