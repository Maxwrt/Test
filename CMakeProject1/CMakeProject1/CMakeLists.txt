# CMakeList.txt: CMakeProject1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
project(CMakeProject1)
set(Boost_DEBUG on)

#根据本地Boost环境修改此值
set(BOOST_ROOT E:\\package\\boost\\)
#使用环境变量 $ENV{name}
#写入环境变量 
set(ENV{BOOST_ROOT} ${BOOST_ROOT})

set(BOOST_INCLUDEDIR ${BOOST_ROOT}boost_1_80_0)
set(BOOST_LIBRARYDIR ${BOOST_ROOT}boost_1_80_0\\stage\\lib)
set(Boost_LIB_PREFIX lib)
find_package(Boost 1.80.0 REQUIRED COMPONENTS regex filesystem log date_time)

set(UTILITY_INCLUDEDIR ${CMAKE_CURRENT_SOURCE_DIR}/utility)
set(UTILITY_LIBNRARYDIR ${CMAKE_CURRENT_SOURCE_DIR}/out/build/bin/)

 
link_directories(${BOOST_LIBRARYDIR})
link_directories(${UTILITY_LIBNRARYDIR})
include_directories(${BOOST_INCLUDEDIR})
include_directories(${UTILITY_INCLUDEDIR})

#工程的根目录
message(STATUS project_source_dir: ${PROJECT_SOURCE_DIR})
#运行cmake命令的目录
message(STATUS project_binary_dir: ${PROJECT_BINARY_DIR})
#返回通过project命令定义的项目名称
message(STATUS paoject_name: ${PROJECT_NAME})
#当前处理的CMakeLists.txt所在的路径
message(STATUS cmake_current_source_dir: ${CMAKE_CURRENT_SOURCE_DIR})
#target编译目录
message(STATUS cmake_current_binary_dir: ${CMAKE_CURRENT_BINARY_DIR})
#CMakeLists的完整路径
message(STATUS cmake_current_list_dir: ${CMAKE_CURRENT_LIST_DIR})
#当前所在行
message(STATUS cmake_current_list_line: ${CMAKE_CURRENT_LIST_LINE})
#定义自己的cmake模块所在的路径 ${CMAKE_MODULE_PATH}
#重新定义目标二进制可执行文件的存放位置 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/out/build/bin)
#重新定义目标链接库文件的存放路径 ${LIBRARY_OUTPUT_PATH}

#cmake主版本号，比如3.4.1中的 3
message(STATUS cmake_major_version: ${CMAKE_MAJOR_VERSION})
#cmake次版本号，比如3.4.1中的 4
message(STATUS cmake_minor_version: ${CMAKE_MINOR_VERSION})
#cmake补丁等级，比如3.4.1中的 1
message(STATUS cmake_patch_version: ${CMAKE_PATCH_VERSION})
#系统名称，比如 Linux-­2.6.22
message(STATUS cmake_system: ${CMAKE_SYSTEM})
#不包含版本的系统名，比如 Linux
message(STATUS cmake_system_name: ${CMAKE_SYSTEM_NAME})
#系统版本，比如 2.6.22
message(STATUS cmake_system_version: ${CMAKE_SYSTEM_VERSION})
#：处理器名称，比如 i686
message(STATUS cmake_system_processor: ${CMAKE_SYSTEM_PROCESSOR})
#UNIX：在所有的类 UNIX 平台下该值为 TRUE，包括 OS X 和 cygwin
#WIN32：在所有的 win32 平台下该值为 TRUE，包括 cygwin
#BUILD_SHARED_LIBS：这个开关用来控制默认的库编译方式，如果不进行设置，使用 add_library 又没有指定库类型的情况下，默认编译生成的库都是静态库。如果 set(BUILD_SHARED_LIBS ON) 后，默认生成的为动态库
#CMAKE_C_FLAGS：设置 C 编译选项，也可以通过指令 add_definitions() 添加
#CMAKE_CXX_FLAGS：设置 C++ 编译选项，也可以通过指令 add_definitions() 添加
#CMAKE_INCLUDE_PATH、CMAKE_LIBRARY_PATH 环境变量
#target_include_directories里的关键字(private、public、interface)控制头文件目录传递
#target_link_libraries里的关键字控制头文件目录以及链接库传递

message(STATUS boost_version: ${Boost_VERSION_STRING})
message(STATUS boost_include_dir: ${BOOST_INCLUDEDIR})
message(STATUS boost_library_dir: ${BOOST_LIBRARYDIR})
message(STATUS utility_include_dir: ${UTILITY_INCLUDEDIR})
message(STATUS utility_library_dir: ${UTILITY_LIBNRARYDIR})
message(STATUS env_boost_root: $ENV{BOOST_ROOT})

add_subdirectory(utility)
# 将源代码添加到此项目的可执行文件。
add_executable (CMakeProject1 "CMakeProject1.cpp" "CMakeProject1.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProject1 PROPERTY CXX_STANDARD 20)
endif()
target_link_libraries(CMakeProject1 PUBLIC ${Boost_LIBRARIES} utility)


#configure_file 命令用于加入一个配置头文件 config.h ，这个文件由 cmake 从 config.h.in 生成，通过这样的机制，将可以通过预定义一些参数和变量来控制代码的生成。
#option 命令添加了一个 USE_MYMATH 选项，并且默认值为 ON 。根据 USE_MYMATH 变量的值来决定是否使用我们自己编写的 MathFunctions 库。

# TODO: 如有需要，请添加测试并安装目标。

# 本地编译可以理解为，在当前编译平台下，编译出来的程序只能放到当前平台下运行。
# 交叉编译可以理解为，在当前编译平台下，编译出来的程序能运行在体系结构不同的另一种目标平台上，但是编译平台本身却不能运行该程序。
# 交叉编译链就是为了编译跨平台体系结构的程序代码而形成的由多个子工具构成的一套完整的工具集。
# 同时，它隐藏了预处理、编译、汇编、链接等细节，当我们指定了源文件(.c)时，它会自动按照编译流程调用不同的子工具，自动生成最终的二进制程序映像(.bin)。
# Linux上通常使用的C函数库为glibc。glibc是linux系统中最底层的api，几乎其它任何运行库都会依赖于glibc。
# glibc除了封装linux操作系统所提供的系统服务外，它本身也提供了许多其它一些必要功能服务的实现
# YMODEM 文件传输协议
# Fatal messages indicate the system is going to terminate to try to protect itself from detriment.
# Error messages indicate significant issues that won’t necessarily damage the system, but are preventing it from functioning properly.
# Warn messages indicate unexpected activity or non-ideal results that might represent a deeper issue, but don’t harm functionality outright.
# Info messages indicate event and status updates that serve as a visual verification that the system is running as expected.
# Debug messages detail the entire step-by-step process of the system execution.