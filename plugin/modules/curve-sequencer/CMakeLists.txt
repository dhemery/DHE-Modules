add_library(curve_sequencer INTERFACE)
target_link_libraries(curve_sequencer
        INTERFACE
        components
        controls
        config
        widgets
        Rack::SDK
        )
