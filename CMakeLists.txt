cmake_minimum_required(VERSION 3.9)

project(main)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

aux_source_directory(. SRC_LIST)

add_executable(main ${SRC_LIST})

target_link_libraries(main ${OpenCV_LIBS})

