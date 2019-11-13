cmake_minimum_required(VERSION 2.9)

project(demo)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

#aux_source_directory(. SRC_LIST)

#add_executable(demo ${SRC_LIST})

add_executable(demo main.cpp constant.h constants.h energy.cpp energy.h options.cpp options.h param_struct_define.h)

target_link_libraries(demo ${OpenCV_LIBS})

