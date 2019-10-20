cmake_minimum_required(VERSION 3.15)

add_library(func OBJECT)
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
set_target_properties(func
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
        )
