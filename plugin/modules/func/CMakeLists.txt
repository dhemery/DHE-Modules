add_library(func
        Func.cpp
        FuncPanel.cpp
        Func6.cpp
        Func6Panel.cpp
        FuncChannel.cpp
        )
target_include_directories(func PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(func
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
