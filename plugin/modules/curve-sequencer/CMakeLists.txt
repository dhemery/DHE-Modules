add_library(curve_sequencer
        CurveSequencer.cpp
        )
target_include_directories(curve_sequencer PUBLIC .)
target_link_libraries(curve_sequencer
        PUBLIC
        controls
        config
        widgets
        Rack::SDK
        )
