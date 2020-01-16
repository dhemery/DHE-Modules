add_library(func INTERFACE)
add_library(dhe::func ALIAS func)
target_link_libraries(func
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )