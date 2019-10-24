add_library(func
        Func.cpp
        FuncPanel.cpp
        Func6.cpp
        Func6Panel.cpp
        FuncChannel.cpp
        )
target_include_directories(func PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(func
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
