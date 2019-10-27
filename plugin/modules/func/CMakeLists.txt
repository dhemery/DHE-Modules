add_library(func STATIC)
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
        components
        controls
        config
        widgets
        Rack::SDK
        )