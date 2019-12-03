add_library(tapers INTERFACE)
add_library(dhe::tapers ALIAS tapers)
target_link_libraries(tapers
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )