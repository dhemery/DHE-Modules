add_library(xycloid INTERFACE)
add_library(dhe::xycloid ALIAS xycloid)
target_link_libraries(xycloid
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )