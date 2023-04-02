cmake_minimum_required(VERSION 3.20)
# cmake_policy(SET CMP0012 NEW)
project(examples)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

message(STATUS "Operation system is ${CMAKE_SYSTEM}")

set(ALL_LIBS glad mylib assimp)

if (WIN32)
    message(STATUS "Windows")
elseif (APPLE)
    message(STATUS "Apple")
    include_directories(3rd-party/glfw-3.3.4.bin.MACOS/include)
    link_directories(3rd-party/glfw-3.3.4.bin.MACOS/lib-x86_64)
    list(APPEND ALL_LIBS glfw.3.dylib "-framework OpenGL")
elseif (UNIX)
    message(STATUS "UNIX-like OS's.")
    list(APPEND ALL_LIBS glfw3 GL Xrandr Xi X11 pthread dl)
endif ()

find_package(assimp REQUIRED)
if (assimp_FOUND)
    link_directories(${ASSIMP_LIBRARY_DIRS})
    include_directories(${ASSIMP_INCLUDE_DIRS})
endif(assimp_FOUND)

include_directories(3rd-party/stb_image)
include_directories(3rd-party/glm)
include_directories(3rd-party/glad/include/)
include_directories(src/)

add_subdirectory(3rd-party/glad)
add_subdirectory(src)
add_subdirectory(example/basic)
add_subdirectory(example/postprocess)
add_subdirectory(example/model)
add_subdirectory(example/skin)