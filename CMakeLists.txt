cmake_minimum_required(VERSION 3.2)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-variable")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(IotaEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
add_library(
    ${PROJECT_NAME}
    SHARED
    ${SRC_DIR}/IotaApplication.cpp
    ${SRC_DIR}/IotaEvent.cpp
    )

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/lib/SDL2
    ${CMAKE_SOURCE_DIR}/lib/SDL2_image)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    SDL2
    SDL2main
    SDL2_image
    )
