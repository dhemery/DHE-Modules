add_library(curve_sequencer STATIC)
target_sources(curve_sequencer
        PRIVATE
        CurveSequencer.cpp
        )
target_link_libraries(curve_sequencer
        PUBLIC
        components
        controls
        config
        widgets
        Rack::SDK
        )