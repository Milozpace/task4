cmake_minimum_required(VERSION 3.10)

project(test)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

set(LIBRARY_OUTPUT_DIRECTORY lib)

#动态库
add_subdirectory(lib)

#生成可执行程序
add_executable(app main.cpp)

target_link_libraries(
    app 
    PRIVATE
    Camera
)

