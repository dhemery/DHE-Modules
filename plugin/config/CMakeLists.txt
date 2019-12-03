add_library(config INTERFACE)
add_library(dhe::config ALIAS config)
target_link_libraries(config
        INTERFACE
        dhe::components
        dhe::controls
        Rack::SDK
        )
