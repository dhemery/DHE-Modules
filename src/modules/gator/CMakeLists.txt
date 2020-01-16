add_library(gator INTERFACE)
add_library(dhe::gator ALIAS gator)
target_link_libraries(gator
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )