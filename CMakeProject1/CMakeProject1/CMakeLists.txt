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
message(project_source_dir: ${PROJECT_SOURCE_DIR})
#运行cmake命令的目录
message(project_binary_dir: ${PROJECT_BINARY_DIR})
#返回通过project命令定义的项目名称
message(paoject_name: ${PROJECT_NAME})
#当前处理的CMakeLists.txt所在的路径
message(cmake_current_source_dir: ${CMAKE_CURRENT_SOURCE_DIR})
#target编译目录
message(cmake_current_binary_dir: ${CMAKE_CURRENT_BINARY_DIR})
#CMakeLists的完整路径
message(cmake_current_list_dir: ${CMAKE_CURRENT_LIST_DIR})
#当前所在行
message(cmake_current_list_line: ${CMAKE_CURRENT_LIST_LINE})
#定义自己的cmake模块所在的路径 ${CMAKE_MODULE_PATH}
#重新定义目标二进制可执行文件的存放位置 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/out/build/bin)
#重新定义目标链接库文件的存放路径 ${LIBRARY_OUTPUT_PATH}

#cmake主版本号，比如3.4.1中的 3
message(cmake_major_version: ${CMAKE_MAJOR_VERSION})
#cmake次版本号，比如3.4.1中的 4
message(cmake_minor_version: ${CMAKE_MINOR_VERSION})
#cmake补丁等级，比如3.4.1中的 1
message(cmake_patch_version: ${CMAKE_PATCH_VERSION})
#系统名称，比如 Linux-­2.6.22
message(cmake_system: ${CMAKE_SYSTEM})
#不包含版本的系统名，比如 Linux
message(cmake_system_name: ${CMAKE_SYSTEM_NAME})
#系统版本，比如 2.6.22
message(cmake_system_version: ${CMAKE_SYSTEM_VERSION})
#：处理器名称，比如 i686
message(cmake_system_processor: ${CMAKE_SYSTEM_PROCESSOR})
#UNIX：在所有的类 UNIX 平台下该值为 TRUE，包括 OS X 和 cygwin
#WIN32：在所有的 win32 平台下该值为 TRUE，包括 cygwin
#BUILD_SHARED_LIBS：这个开关用来控制默认的库编译方式，如果不进行设置，使用 add_library 又没有指定库类型的情况下，默认编译生成的库都是静态库。如果 set(BUILD_SHARED_LIBS ON) 后，默认生成的为动态库
#CMAKE_C_FLAGS：设置 C 编译选项，也可以通过指令 add_definitions() 添加
#CMAKE_CXX_FLAGS：设置 C++ 编译选项，也可以通过指令 add_definitions() 添加
#CMAKE_INCLUDE_PATH、CMAKE_LIBRARY_PATH 环境变量
#target_include_directories里的关键字(private、public、interface)控制头文件目录传递
#target_link_libraries里的关键字控制头文件目录以及链接库传递

message(boost_version: ${Boost_VERSION_STRING})
message(boost_include_dir: ${BOOST_INCLUDEDIR})
message(boost_library_dir: ${BOOST_LIBRARYDIR})
message(utility_include_dir: ${UTILITY_INCLUDEDIR})
message(utility_library_dir: ${UTILITY_LIBNRARYDIR})
message(env_boost_root: $ENV{BOOST_ROOT})

#add_subdirectory(utility)
# 将源代码添加到此项目的可执行文件。
add_executable (CMakeProject1 "CMakeProject1.cpp" "CMakeProject1.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProject1 PROPERTY CXX_STANDARD 20)
endif()
target_link_libraries(CMakeProject1 PUBLIC ${Boost_LIBRARIES} utility)


#configure_file 命令用于加入一个配置头文件 config.h ，这个文件由 cmake 从 config.h.in 生成，通过这样的机制，将可以通过预定义一些参数和变量来控制代码的生成。
#option 命令添加了一个 USE_MYMATH 选项，并且默认值为 ON 。根据 USE_MYMATH 变量的值来决定是否使用我们自己编写的 MathFunctions 库。

# TODO: 如有需要，请添加测试并安装目标。
