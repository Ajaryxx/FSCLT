cmake_minimum_required(VERSION 3.31.6)


project(FSCLT
VERSION 0.1.0
LANGUAGES CXX)

#global settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#proj varables
set(PROJ_EXE "FSCLT")


add_executable(${PROJ_EXE} "src/main.cpp")
target_include_directories(${PROJ_EXE} PRIVATE "include/" ${CMAKE_BINARY_DIR}/config)
target_precompile_headers(${PROJ_EXE} PRIVATE "include/PCH.hpp")

configure_file("include/config.hpp.in"
${CMAKE_BINARY_DIR}/config/config.hpp ESCAPE_QUOTES
)

add_subdirectory(src)
