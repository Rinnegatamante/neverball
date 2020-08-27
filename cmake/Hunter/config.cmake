if(ANDROID)
hunter_config(
    SDL2
    VERSION 2.0.7-p3
)
elseif(APPLE AND NOT IOS)
hunter_config(
    SDL2
    VERSION 2.0.12-p0
)
elseif(IOS)
hunter_config(
    SDL2
    VERSION 2.0.12-p0
    CMAKE_ARGS "HIDAPI=NO"
)
endif()
