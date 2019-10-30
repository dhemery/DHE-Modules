add_library(blossom STATIC)
add_library(dhe::blossom ALIAS blossom)
target_sources(blossom
        PRIVATE
        Blossom.cpp
        BlossomPanel.cpp
        )
target_link_libraries(blossom
        PUBLIC
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )