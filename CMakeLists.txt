cmake_minimum_required(VERSION 3.5)
project(opencvtest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)
include_directories(${OPENCV_INCLUDE_DIR})


find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})

find_package( Threads )

set(SOURCE_FILES)

add_executable(opencvtest main.cpp life.cpp)

target_link_libraries(opencvtest ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${X11_LIBRARIES})
