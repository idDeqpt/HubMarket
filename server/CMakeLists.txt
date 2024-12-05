cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT_NAME HubMarket_server)

project(${PROJECT_NAME})


add_executable(${PROJECT_NAME}
	source/main.cpp
	source/network/Address.cpp
	source/network/URL.cpp
	source/network/HTTP.cpp
	source/network/HTTPServer.cpp
	bt.rc
)


set(dir ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir})
set(CMAKE_BUILD_FILES_DIRECTORY ${dir})
set(CMAKE_BUILD_DIRECTORY ${dir})
set(CMAKE_BINARY_DIR  ${dir})
SET(EXECUTABLE_OUTPUT_PATH ${dir})
SET(LIBRARY_OUTPUT_PATH ${dir}lib)
SET(CMAKE_CACHEFILE_DIR ${dir})