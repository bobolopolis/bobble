project(bobble)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

find_package( OpenCV REQUIRED)
target_link_libraries(bobble gps pthread ${OpenCV_LIBS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
