cmake_minimum_required(VERSION 3.26)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

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
	${SRC_DIR}/IotaGameInstance.cpp
	${SRC_DIR}/IotaBasic.cpp
	${SRC_DIR}/IotaTexture.cpp
	)

target_include_directories(${PROJECT_NAME} PUBLIC 
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include/SDL2
	${CMAKE_SOURCE_DIR}/include/SDL2_image
	)

target_link_directories(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_SOURCE_DIR}/lib
	${CMAKE_SOURCE_DIR}/lib/SDL2
	${CMAKE_SOURCE_DIR}/lib/SDL2_image
	)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
	SDL2
	SDL2main
	SDL2_image
	)