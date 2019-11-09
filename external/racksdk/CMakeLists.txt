include(FetchContent)

FetchContent_Declare(racksdk
        URL https://vcvrack.com/downloads/Rack-SDK-1.1.6.zip
        )
FetchContent_GetProperties(racksdk)
if (NOT racksdk_POPULATED)
    FetchContent_Populate(racksdk)
endif ()
add_library(racksdk INTERFACE IMPORTED GLOBAL)
target_include_directories(racksdk
        INTERFACE
        ${racksdk_SOURCE_DIR}/include
        ${racksdk_SOURCE_DIR}/dep/include
        )
target_compile_features(racksdk INTERFACE cxx_std_11)
if (NOT UNIX)
    target_link_directories(racksdk INTERFACE ${racksdk_SOURCE_DIR})
    target_link_libraries(racksdk INTERFACE Rack)
endif ()

add_library(Rack::SDK ALIAS racksdk)
