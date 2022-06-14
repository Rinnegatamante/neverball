#fix SDL2 hidapi/android/hid.cpp linking error
if(ANDROID AND NOT ${CMAKE_ANDROID_ARCH_ABI} STREQUAL "arm64-v8a")
hunter_config(
    SDL2
    VERSION 2.0.18
    CMAKE_ARGS
        CMAKE_CXX_FLAGS=-fPIC
)
elseif(IOS)
hunter_config(
    SDL2
    VERSION 2.0.22-22d6e09
    CMAKE_ARGS "SDL_HIDAPI=NO"
)
endif()
