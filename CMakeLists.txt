cmake_minimum_required(VERSION 3.20)

project(vee LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(ProjectSettings)
include(CompilerOptions)
include(Dependencies)
include(Testing)

add_subdirectory(core)
add_subdirectory(veec)
