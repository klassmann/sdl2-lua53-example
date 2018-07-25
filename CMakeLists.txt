cmake_minimum_required (VERSION 2.8.11)
project (example)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -ggdb ${CMAKE_CXX_FLAGS}")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(Lua REQUIRED)

include_directories(
	"include/"
	${SDL2_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIR}
	${LUA_INCLUDE_DIR}
)


set(SOURCE_FILES
	"src/main.c"
)

configure_file(
	"src/settings.lua"
	"settings.lua"
)

configure_file(
	"src/script.lua"
	"script.lua"
)

add_executable (${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
	${SDL2_LIBRARY}
	${SDL2_TTF_LIBRARY}
	${SDL2_IMAGE_LIBRARY}
	${LUA_LIBRARIES}
)