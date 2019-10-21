add_library(func)
target_include_directories(func PUBLIC .)
target_sources(func
        PRIVATE
        Func.cpp
        FuncPanel.cpp
        Func6.cpp
        Func6Panel.cpp
        FuncChannel.cpp
        )
target_link_libraries(func
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
