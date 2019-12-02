add_library(blossom INTERFACE)
add_library(dhe::blossom ALIAS blossom)
target_link_libraries(blossom
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )