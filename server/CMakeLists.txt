cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT_NAME HubMarket_server)
set(PATH_TO_LIB ../lib)

project(${PROJECT_NAME})

include_directories(${PATH_TO_LIB})

add_executable(${PROJECT_NAME}
	source/main.cpp
	${PATH_TO_LIB}/Timer.cpp
	${PATH_TO_LIB}/network/Address.cpp
	${PATH_TO_LIB}/network/URL.cpp
	${PATH_TO_LIB}/network/HTTP.cpp
	${PATH_TO_LIB}/network/ServerSessionData.cpp
	${PATH_TO_LIB}/network/TCPServer.cpp
	${PATH_TO_LIB}/network/TCPClient.cpp
	${PATH_TO_LIB}/network/HTTPServer.cpp
	${PATH_TO_LIB}/network/HTTPRequests.cpp
	bt.rc
)


target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
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