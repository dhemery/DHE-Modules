include(FetchContent)

include(GoogleTest)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
)
FetchContent_GetProperties(googletest)
if (NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif ()

FetchContent_Declare(racksdk
        URL https://vcvrack.com/downloads/Rack-SDK-1.1.5.zip
        )
FetchContent_GetProperties(racksdk)
if (NOT racksdk_POPULATED)
    FetchContent_Populate(racksdk)
endif ()
add_library(racksdk INTERFACE)
target_include_directories(racksdk
        INTERFACE
        ${racksdk_SOURCE_DIR}/include
        ${racksdk_SOURCE_DIR}/dep/include
        )
target_compile_features(racksdk INTERFACE cxx_std_11)

