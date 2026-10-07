cmake_minimum_required(VERSION 3.10)

project(test)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_subdirectory(lib)

add_executable(app main.cpp)

target_link_libraries(
    app
    PRIVATE
    Camera
)

