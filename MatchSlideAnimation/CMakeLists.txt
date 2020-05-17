cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0054 NEW)
project(MatchSlide CXX)
set(CMAKE_CXX_STANDARD 14)
add_subdirectory(lib/FFmpeg)

add_subdirectory(NetworkAPI)
add_subdirectory(External)
find_package(PkgConfig REQUIRED)

find_package(OpenGL REQUIRED)

# Look for pthread && spdlog
find_package(Threads)

# Look for Truetype && libpng
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

pkg_check_modules(PNG REQUIRED libpng)
include_directories(${PNG_INCLUDE_DIRS})

# Look for GL requirements
pkg_check_modules(LIBEPOXY REQUIRED epoxy)
include_directories(${LIBEPOXY_INCLUDE_DIR})
pkg_check_modules(LIBDRM REQUIRED libdrm)
include_directories(${LIBDRM_INCLUDE_DIRS})
pkg_check_modules(GBM REQUIRED gbm)
include_directories(${GBM_INCLUDE_DIRS})
pkg_check_modules(EGL REQUIRED egl)
include_directories(${EGL_INCLUDE_DIRS})
# Set necessary link libraries
set(REQUIRED_EXTERNAL_LIBS
        ${CMAKE_THREAD_LIBS_INIT}
        ${LIBEPOXY_LIBRARIES}
        ${LIBDRM_LIBRARIES}
        ${GBM_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        ${PNG_LIBRARIES}
        )
add_definitions(-DGL_SILENCE_DEPRECATION)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -O0 -g -pg -lglfw -lGLEW -lGLU -lGL -lglut -lfreetype -lpng -lpthread -lX11") #-lSOIL -lpng")

add_library(RenderEngine_lib
        basicimage.cpp
        SpriteRenderer.cpp ResourceManager.cpp
        shader.cpp texture.cpp
        TextRenderer.cpp
        VideoRender.cpp
        Effects.cpp
        Event.cpp
        )


add_executable(${PROJECT_NAME}  main.cpp  )
target_link_libraries(${PROJECT_NAME} RenderEngine_lib)
target_link_libraries(${PROJECT_NAME} JSONAPI_lib netLink)
target_link_libraries(${PROJECT_NAME} jsonxx_lib)
target_link_libraries(${PROJECT_NAME} FFmpeg)
target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++)
