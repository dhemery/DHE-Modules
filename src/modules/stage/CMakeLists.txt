add_library(stage INTERFACE)
add_library(dhe::stage ALIAS stage)
target_link_libraries(stage
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )