cmake_minimum_required(VERSION 3.10)

project(test)

set(CMAKE_CXX_STANDARD 17)


add_executable(test main.cpp)


target_include_directories(test 
    PRIVATE 
    /opt/MVS/include)

target_link_libraries(test 
    /opt/MVS/lib/64/libMvCameraControl.so)
