cmake_minimum_required(VERSION 3.26)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(Iota)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
add_library(
	IotaEngine
	SHARED
	${SRC_DIR}/IotaApplication.cpp
	${SRC_DIR}/IotaGameInstance.cpp
	${SRC_DIR}/IotaBasic.cpp
	${SRC_DIR}/IotaEvent.cpp
	${SRC_DIR}/IotaScriptEnvironment.cpp
	${SRC_DIR}/IotaTexture.cpp
	${SRC_DIR}/IotaModel.cpp
	)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
target_include_directories(IotaEngine 
	PRIVATE
	${SRC_DIR}
	${SRC_DIR}/libraries
	${LIB_DIR}/lua/include
	${LIB_DIR}/SDL2/include
	${LIB_DIR}/SDL2_image/include
	)

target_link_directories(
	IotaEngine
	PRIVATE
	${LIB_DIR}/lua
	${LIB_DIR}/SDL2
	${LIB_DIR}/SDL2_image
	)

target_link_libraries(
	IotaEngine
	PRIVATE
	lua54
	SDL2
	SDL2main
	SDL2_image
	)

add_executable(Iota ${CMAKE_SOURCE_DIR}/test/test.cpp)

target_include_directories(Iota 
	PRIVATE
	${SRC_DIR}
	${SRC_DIR}/libraries
	${LIB_DIR}/lua/include
	${LIB_DIR}/SDL2/include
	${LIB_DIR}/SDL2_image/include
	)

target_link_libraries(
	Iota
	PRIVATE
	IotaEngine
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	if (WIN32)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	endif()
endif()

add_dependencies(Iota IotaEngine)
