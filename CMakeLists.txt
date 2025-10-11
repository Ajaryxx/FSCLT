cmake_minimum_required(VERSION 3.31.6)


project(FSCLT
VERSION 0.1.0
LANGUAGES CXX)

#global settings
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

#proj varables
set(PROJ_EXE "FSCLT")


add_executable(${PROJ_EXE} "src/main.cpp")
target_include_directories(${PROJ_EXE} PRIVATE "include/")
target_precompile_headers(${PROJ_EXE} PRIVATE "include/PCH.hpp")