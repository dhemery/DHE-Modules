add_library(func
        Func.cpp
        FuncPanel.cpp
        Func6.cpp
        Func6Panel.cpp
        FuncChannel.cpp
        )
target_include_directories(cubic PUBLIC .)
target_link_libraries(func
        PUBLIC
        components
        controls
        config
        widgets
        Rack::SDK
        )
