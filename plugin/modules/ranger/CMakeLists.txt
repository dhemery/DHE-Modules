add_library(ranger INTERFACE)
add_library(dhe::ranger ALIAS ranger)
target_link_libraries(ranger
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )