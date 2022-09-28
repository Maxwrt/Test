# CMakeList.txt: CMakeProject1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project(utility)
add_compile_definitions(BUILD_DLL)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../out/build/bin)
message(STATUS utility.lib output path: ${LIBRARY_OUTPUT_PATH})
add_library(utility SHARED math_.cpp)