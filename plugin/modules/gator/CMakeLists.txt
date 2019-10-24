add_library(gator
        Gator.cpp
        GatorPanel.cpp
        )
target_include_directories(gator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gator
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
