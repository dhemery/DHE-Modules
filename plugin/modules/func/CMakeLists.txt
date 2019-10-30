add_library(func STATIC)
add_library(dhe::func ALIAS func)
target_sources(func
        PRIVATE
        Func.cpp
        Func6.cpp
        Func6Panel.cpp
        FuncChannel.cpp
        FuncPanel.cpp
        )
target_link_libraries(func
        PUBLIC
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )