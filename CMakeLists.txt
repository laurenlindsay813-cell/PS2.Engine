cmake_minimum_required(VERSION 3.16)
project(PS2Engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ps2core_smoke
    tests/ps2core_smoke.cpp
    PS2Core.cpp
    PS2CoreInstructions.cpp
    PS2CoreMemory.cpp
)

target_include_directories(ps2core_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
