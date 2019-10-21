add_library(tapers
        Tapers.cpp
        TapersPanel.cpp
        )
target_include_directories(tapers PUBLIC .)
target_link_libraries(tapers
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
