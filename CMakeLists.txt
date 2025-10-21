cmake_minimum_required(VERSION 3.31.6)


project(FSCLT
VERSION 0.1.0
LANGUAGES CXX)

#global settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#proj varables
set(PROJ_EXE "FSCLT")


add_executable(${PROJ_EXE} "src/main.cpp")
target_include_directories(${PROJ_EXE} PRIVATE "include/")
target_precompile_headers(${PROJ_EXE} PRIVATE "include/PCH.hpp")

add_subdirectory(src)
