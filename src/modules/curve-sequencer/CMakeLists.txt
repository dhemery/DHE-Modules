add_library(curve_sequencer INTERFACE)

target_sources(curve_sequencer INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/AdvanceMode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CurveSequencer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CurveSequencerControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CurveSequencerModule.h
        ${CMAKE_CURRENT_SOURCE_DIR}/CurveSequencerPanel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/StepController.h
        ${CMAKE_CURRENT_SOURCE_DIR}/StepEvent.h
        ${CMAKE_CURRENT_SOURCE_DIR}/GenerateMode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/StepSelector.h
        )

target_include_directories(curve_sequencer INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

target_link_libraries(curve_sequencer
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )

add_library(dhe::curve_sequencer ALIAS curve_sequencer)
