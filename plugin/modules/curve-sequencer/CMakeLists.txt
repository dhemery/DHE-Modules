add_library(curve_sequencer INTERFACE)
add_library(dhe::curve_sequencer ALIAS curve_sequencer)
target_link_libraries(curve_sequencer
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )
