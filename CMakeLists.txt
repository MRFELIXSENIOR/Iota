cmake_minimum_required(VERSION 3.26)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Iota)

# Main

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
add_library(
	Iota
	SHARED
	${SRC_DIR}/IotaException.cpp
	${SRC_DIR}/IotaApplication.cpp
	${SRC_DIR}/IotaBehavior.cpp
	${SRC_DIR}/IotaGameInstance.cpp
	${SRC_DIR}/IotaEvent.cpp
	${SRC_DIR}/IotaBasic.cpp
	${SRC_DIR}/IotaKeyboard.cpp
	${SRC_DIR}/IotaScriptEnvironment.cpp
	${SRC_DIR}/IotaTexture.cpp
	${SRC_DIR}/IotaModel.cpp
	${SRC_DIR}/IotaFont.cpp
	)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
target_include_directories(
	Iota 
	PRIVATE
	${SRC_DIR}
	${SRC_DIR}/libraries
	${LIB_DIR}/lua/include
	${LIB_DIR}/SDL2/include
	${LIB_DIR}/SDL2_image/include
	${LIB_DIR}/SDL2_mixer/include
	${LIB_DIR}/SDL2_ttf/include
	${LIB_DIR}/SDL2_gfx/include
	)

target_link_directories(
	Iota
	PRIVATE
	${LIB_DIR}/lua
	${LIB_DIR}/SDL2
	${LIB_DIR}/SDL2_image
	${LIB_DIR}/SDL2_mixer
	${LIB_DIR}/SDL2_ttf
	${LIB_DIR}/SDL2_gfx
	)

target_link_libraries(
	Iota
	PRIVATE
	lua54
	SDL2
	SDL2main
	SDL2_image
	SDL2_mixer
	SDL2_ttf
	SDL2_gfx
	)

add_executable(IotaTest test/main.cpp)

if (WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

target_include_directories(
	IotaTest
	PUBLIC
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/src/libraries
	${CMAKE_SOURCE_DIR}/lib/lua/include
	${CMAKE_SOURCE_DIR}/lib/SDL2/include
	${CMAKE_SOURCE_DIR}/lib/SDL2_image/include
)

target_link_libraries(
	IotaTest
	PUBLIC
	Iota
)

add_dependencies(IotaTest Iota)