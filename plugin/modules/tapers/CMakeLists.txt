add_library(tapers)
target_include_directories(tapers PUBLIC .)
target_sources(tapers
        PRIVATE
        Tapers.cpp
        TapersPanel.cpp
        )
target_link_libraries(tapers
        PUBLIC
        components
        controls
        config
        widgets
        racksdk
        )
