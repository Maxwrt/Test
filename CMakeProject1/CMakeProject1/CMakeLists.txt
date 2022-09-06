# CMakeList.txt: CMakeProject1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
project(CMakeProject1)
set(Boost_DEBUG on)
set(BOOST_INCLUDEDIR C:\\app\\Boost\\boost_1_80_0)
set(BOOST_LIBRARYDIR C:\\app\\Boost\\boost_1_80_0\\stage\\lib)
set(Boost_LIB_PREFIX lib)
 
find_package(Boost 1.80.0 REQUIRED COMPONENTS regex)
 
link_directories(${BOOST_LIBRARYDIR})
include_directories(${BOOST_INCLUDEDIR})
 
 
message(Boost\ version: ${Boost_VERSION_STRING})
message(Boost\ include\ dir: ${Boost_INCLUDE_DIRS})
message(Boost\ library\ dir: ${Boost_LIBRARY_DIRS})

# 将源代码添加到此项目的可执行文件。
add_executable (CMakeProject1 "CMakeProject1.cpp" "CMakeProject1.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProject1 PROPERTY CXX_STANDARD 20)
endif()
target_link_libraries(CMakeProject1 ${Boost_LIBRARIES})


# TODO: 如有需要，请添加测试并安装目标。
