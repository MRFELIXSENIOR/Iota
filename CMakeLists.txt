cmake_minimum_required(VERSION 3.26)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Iota)

# Main

set(MONO_ROOT "C:/Program Files/Mono")
set(MONO_INCLUDE_DIR "${MONO_ROOT}/include/mono-2.0")
set(MONO_LIB "${MONO_ROOT}/lib")

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
add_library(
	IotaEngine
	SHARED
	${SRC_DIR}/IotaException.cpp
	${SRC_DIR}/IotaApplication.cpp
	${SRC_DIR}/IotaBehavior.cpp
	${SRC_DIR}/IotaGameInstance.cpp
	${SRC_DIR}/IotaMono.cpp
	${SRC_DIR}/IotaEvent.cpp
	${SRC_DIR}/IotaDefs.cpp
	${SRC_DIR}/IotaBasic.cpp
	${SRC_DIR}/IotaKeyboard.cpp
	${SRC_DIR}/IotaTexture.cpp
	${SRC_DIR}/IotaModel.cpp
	${SRC_DIR}/IotaFont.cpp
	)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

include_directories(
	${SRC_DIR}
	${SRC_DIR}/libraries
	${LIB_DIR}/SDL2/include
	${LIB_DIR}/SDL2_image/include
	${LIB_DIR}/SDL2_mixer/include
	${LIB_DIR}/SDL2_ttf/include
	${LIB_DIR}/SDL2_gfx/include
	${MONO_INCLUDE_DIR}
)

target_link_directories(
	IotaEngine
	PRIVATE
	${LIB_DIR}/SDL2
	${LIB_DIR}/SDL2_image
	${LIB_DIR}/SDL2_gfx
	${LIB_DIR}/SDL2_mixer
	${LIB_DIR}/SDL2_ttf
	${MONO_LIB}
)

target_link_libraries(
	IotaEngine
	PRIVATE
	SDL2
	SDL2main
	SDL2_image
	SDL2_mixer
	SDL2_ttf
	SDL2_gfx
	mono-2.0-sgen
	)

add_executable(Iota ${SRC_DIR}/main.cpp)

if (WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

target_link_libraries(
	Iota
	PRIVATE
	IotaEngine
)

add_dependencies(Iota IotaEngine)