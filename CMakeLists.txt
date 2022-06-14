cmake_minimum_required(VERSION 3.9.2)

# For Android Studio we will force generated toolchain
if(ANDROID_STUDIO)
    set(gen_toolchain "${CMAKE_CURRENT_BINARY_DIR}/generated/toolchain.cmake")
    configure_file(
            "${CMAKE_CURRENT_LIST_DIR}/cmake/template/toolchain.cmake.in"
            "${gen_toolchain}"
            @ONLY
    )
    set(CMAKE_TOOLCHAIN_FILE "${gen_toolchain}" CACHE PATH "" FORCE)
endif()

option(CMAKE_VERBOSE_MAKEFILE "Verbose makefile" OFF)

option(HUNTER_KEEP_PACKAGE_SOURCES "Keep third party sources" ON)
option(HUNTER_STATUS_DEBUG "Print debug info" OFF)

# Neverball options
set(USERDIR   ".neverball" CACHE STRING "User Dir")
set(DATADIR   "./data"     CACHE STRING "Data Dir")
set(LOCALEDIR "./locale"   CACHE STRING "Locale Dir")
set(ENABLE_FS "stdio"      CACHE STRING "File system")

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/drodin/hunter/archive/refs/heads/pr.sdl_master.zip"
    SHA1 "174ff12548667689f4f4d18db3fa1c152552fa3e"
    LOCAL
)

project(neverball)

if(ANDROID)
    set(OPENGL_LIBRARIES -lGLESv1_CM)
elseif(IOS)
    set(OPENGL_LIBRARIES "-framework OpenGLES")
else()
    find_package(OpenGL MODULE REQUIRED)
    if (NOT APPLE)
        find_package(intl)
    endif()
endif()

hunter_add_package(SDL2)
hunter_add_package(SDL_ttf)
hunter_add_package(JPEG)
hunter_add_package(vorbis)
hunter_add_package(PNG)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL_ttf CONFIG REQUIRED)
find_package(JPEG CONFIG REQUIRED)
find_package(vorbis CONFIG REQUIRED)
find_package(PNG CONFIG REQUIRED)

get_target_property(SDL_ttf_INCLUDE_DIRS SDL_ttf::SDL_ttf INTERFACE_INCLUDE_DIRECTORIES)
set(SDL_ttf_INCLUDE_DIRS "${SDL_ttf_INCLUDE_DIRS}/SDL_ttf")

set(MAPC_SRC
    share/vec3.c
    share/base_image.c
    share/solid_base.c
    share/binary.c
    share/log.c
    share/base_config.c
    share/common.c
    share/fs_common.c
    share/fs_png.c
    share/fs_jpg.c
    share/dir.c
    share/array.c
    share/list.c
    share/mapc.c
)

set(BALL_SRC
    share/lang.c
    share/st_common.c
    share/vec3.c
    share/base_image.c
    share/image.c
    share/solid_base.c
    share/solid_vary.c
    share/solid_draw.c
    share/solid_all.c
    share/mtrl.c
    share/part.c
    share/geom.c
    share/ball.c
    share/gui.c
    share/font.c
    share/theme.c
    share/base_config.c
    share/config.c
    share/video.c
    share/glext.c
    share/binary.c
    share/state.c
    share/audio.c
    share/text.c
    share/common.c
    share/list.c
    share/queue.c
    share/cmd.c
    share/array.c
    share/dir.c
    share/fbo.c
    share/glsl.c
    share/fs_common.c
    share/fs_png.c
    share/fs_jpg.c
    share/fs_ov.c
    share/log.c
    share/joy.c
    ball/hud.c
    ball/game_common.c
    ball/game_client.c
    ball/game_server.c
    ball/game_proxy.c
    ball/game_draw.c
    ball/score.c
    ball/level.c
    ball/progress.c
    ball/set.c
    ball/demo.c
    ball/demo_dir.c
    ball/util.c
    ball/st_conf.c
    ball/st_demo.c
    ball/st_save.c
    ball/st_goal.c
    ball/st_fail.c
    ball/st_done.c
    ball/st_level.c
    ball/st_over.c
    ball/st_play.c
    ball/st_set.c
    ball/st_start.c
    ball/st_title.c
    ball/st_help.c
    ball/st_name.c
    ball/st_shared.c
    ball/st_pause.c
    ball/st_ball.c
    ball/main.c
    share/solid_sim_sol.c
    share/tilt_null.c
    share/hmd_null.c
)

set(PUTT_SRC
    share/lang.c
    share/st_common.c
    share/vec3.c
    share/base_image.c
    share/image.c
    share/solid_base.c
    share/solid_vary.c
    share/solid_draw.c
    share/solid_all.c
    share/mtrl.c
    share/part.c
    share/geom.c
    share/ball.c
    share/base_config.c
    share/config.c
    share/video.c
    share/glext.c
    share/binary.c
    share/audio.c
    share/state.c
    share/gui.c
    share/font.c
    share/theme.c
    share/text.c
    share/common.c
    share/list.c
    share/fs_common.c
    share/fs_png.c
    share/fs_jpg.c
    share/fs_ov.c
    share/dir.c
    share/fbo.c
    share/glsl.c
    share/array.c
    share/log.c
    share/joy.c
    putt/hud.c
    putt/game.c
    putt/hole.c
    putt/course.c
    putt/st_all.c
    putt/st_conf.c
    putt/main.c
    share/solid_sim_sol.c
    share/tilt_null.c
    share/hmd_null.c
)

if(ENABLE_FS STREQUAL "stdio")
    list(APPEND MAPC_SRC share/fs_stdio.c)
    list(APPEND BALL_SRC share/fs_stdio.c)
    list(APPEND PUTT_SRC share/fs_stdio.c)
elseif(ENABLE_FS STREQUAL "physfs")
    list(APPEND MAPC_SRC share/fs_physfs.c)
    list(APPEND BALL_SRC share/fs_physfs.c)
    list(APPEND PUTT_SRC share/fs_physfs.c)
endif()

if(ANDROID)
    add_library(neverball SHARED ${BALL_SRC})
    add_library(neverputt SHARED ${PUTT_SRC})

    add_definitions(-DVERSION=\"${VERSION_NAME}\")
    add_definitions(-D__MOBILE__=1)

    set(USERDIR   "user")
    set(DATADIR   "data")
    set(LOCALEDIR "locale")

    set(ENABLE_OPENGLES ON)
    set(ENABLE_NLS OFF)

    set_target_properties(neverball neverputt PROPERTIES DEBUG_POSTFIX "")
elseif(APPLE)
    file(GLOB SHARE_DATA RELATIVE ${CMAKE_CURRENT_LIST_DIR} assets/share/data/*)
    file(GLOB BALL_DATA RELATIVE ${CMAKE_CURRENT_LIST_DIR} assets/ball/data/*)
    file(GLOB PUTT_DATA RELATIVE ${CMAKE_CURRENT_LIST_DIR} assets/putt/data/*)

if(IOS)
    add_executable(neverball MACOSX_BUNDLE ${BALL_SRC} ${BALL_DATA} ${SHARE_DATA} apple/ios/neverball/Assets.xcassets apple/ios/neverball/LaunchScreen.storyboard)
    add_executable(neverputt MACOSX_BUNDLE ${PUTT_SRC} ${PUTT_DATA} ${SHARE_DATA} apple/ios/neverputt/Assets.xcassets apple/ios/neverputt/LaunchScreen.storyboard)

    set(USERDIR "Documents/.neverball")

    add_definitions(-D__MOBILE__=1)
    set(ENABLE_OPENGLES ON)

    set_target_properties(neverball neverputt PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/cmake/template/iOSBundleInfo.plist.in"
        XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
        XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
    )

    set_target_properties(neverball PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "Neverball"
    )

    set_target_properties(neverputt PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "Neverputt"
    )

    set_source_files_properties(apple/ios/neverball/Assets.xcassets PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_source_files_properties(apple/ios/neverputt/Assets.xcassets PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_source_files_properties(apple/ios/neverball/LaunchScreen.storyboard PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_source_files_properties(apple/ios/neverputt/LaunchScreen.storyboard PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
else()
    add_executable(neverball MACOSX_BUNDLE ${BALL_SRC} ${BALL_DATA} ${SHARE_DATA} apple/macos/neverball.icns apple/macos/neverball.entitlements)
    add_executable(neverputt MACOSX_BUNDLE ${PUTT_SRC} ${PUTT_DATA} ${SHARE_DATA} apple/macos/neverputt.icns apple/macos/neverputt.entitlements)

    set(MACOSX_BUNDLE_APP_CATEGORY "public.app-category.action-games")

    set_target_properties(neverball neverputt PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/cmake/template/MacOSXBundleInfo.plist.in"
        MACOSX_BUNDLE_COPYRIGHT "Copyright © 2022 Dmitry Rodin. All rights reserved."
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Mac Developer"
        XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES
    )

    set_target_properties(neverball PROPERTIES
        OUTPUT_NAME "Neverball"
        MACOSX_BUNDLE_ICON_FILE "neverball"
        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "apple/macos/neverball.entitlements"
    )

    set_target_properties(neverputt PROPERTIES
        OUTPUT_NAME "Neverputt"
        MACOSX_BUNDLE_ICON_FILE "neverputt"
        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "apple/macos/neverputt.entitlements"
    )

    set_source_files_properties(apple/macos/neverball.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_source_files_properties(apple/macos/neverputt.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()
    set(ENABLE_NLS OFF)

    set(VERSION_NAME "1.6.003")
    target_compile_definitions(neverball PRIVATE VERSION=\"${VERSION_NAME}\")
    target_compile_definitions(neverputt PRIVATE VERSION=\"${VERSION_NAME}\")

    set_target_properties(neverball neverputt PROPERTIES
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION_NAME}"
        MACOSX_BUNDLE_BUNDLE_VERSION "3"
        XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
        XCODE_ATTRIBUTE_SKIP_INSTALL "No"
        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "$ENV{CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM}"
    )

    set_target_properties(neverball PROPERTIES
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.drodin.neverball"
    )

    set_target_properties(neverputt PROPERTIES
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.drodin.neverputt"
    )

    set_source_files_properties(${SHARE_DATA} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/data")
    set_source_files_properties(${BALL_DATA} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/data")
    set_source_files_properties(${PUTT_DATA} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/data")
else()
    add_executable(neverball ${BALL_SRC})
    add_executable(neverputt ${PUTT_SRC})
endif()

set_property(GLOBAL PROPERTY C_STANDARD 99)

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_C_FLAGS "-Wall -Wshadow -pedantic ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions ${CMAKE_CXX_FLAGS}")
endif()

add_definitions(-DCONFIG_USER=\"${USERDIR}\")
add_definitions(-DCONFIG_DATA=\"${DATADIR}\")
add_definitions(-DCONFIG_LOCALE=\"${LOCALEDIR}\")

if(ENABLE_OPENGLES)
	add_definitions(-DENABLE_OPENGLES=1)
endif()

if(ENABLE_NLS)
	add_definitions(-DENABLE_NLS=1)
else()
	add_definitions(-DENABLE_NLS=0)
endif()

if(ENABLE_HMD STREQUAL "openhmd")
	add_definitions(-DENABLE_HMD=1)
endif()
if(ENABLE_HMD STREQUAL "libovr")
	add_definitions(-DENABLE_HMD=1)
endif()

if(ENABLE_RADIANT_CONSOLE)
	add_definitions(-DENABLE_RADIANT_CONSOLE=1)
endif()

if(ENABLE_TILT STREQUAL "wii")
	# -fms-extensions because
	# libwiimote headers make heavy use of the "unnamed fields" GCC
	# extension.
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_C_FLAGS "-fms-extensions ${CMAKE_C_FLAGS}")
    endif()
endif()

include_directories(
    share
    ${OPENGL_INCLUDE_DIR}
    ${SDL_ttf_INCLUDE_DIRS}
    ${Intl_INCLUDE_DIRS}
)

if(NOT (ANDROID OR IOS))
    add_executable(mapc ${MAPC_SRC})

    target_link_libraries(mapc
        SDL2::SDL2
        JPEG::jpeg
        PNG::png
    )
endif()

set(LIBS
    ${OPENGL_LIBRARIES}
    ${Intl_LIBRARIES}
    SDL2::SDL2
    SDL2::SDL2main
    SDL_ttf::SDL_ttf
    JPEG::jpeg
    vorbis::vorbisfile
)

target_link_libraries(neverball ${LIBS})
target_link_libraries(neverputt ${LIBS})

