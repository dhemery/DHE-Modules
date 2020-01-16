include(FetchContent)
include(GoogleTest)
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
        )

FetchContent_Declare(racksdk
        URL https://vcvrack.com/downloads/Rack-SDK-1.1.6.zip
        )

add_subdirectory(googletest)
add_subdirectory(racksdk)
