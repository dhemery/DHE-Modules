add_library(cubic INTERFACE)
add_library(dhe::cubic ALIAS cubic)
target_link_libraries(cubic
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )