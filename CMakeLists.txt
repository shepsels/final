cmake_minimum_required(VERSION 3.5)
project(finalProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp)
add_executable(finalProject ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(binary_name ${OpenCV_LIBS})