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


#生成测试程序
#[[add_executable(test test.cpp)

target_include_directories(
    test 
    PRIVATE 
    /opt/MVS/include
    )

target_link_libraries(
    test 
    /opt/MVS/lib/64/libMvCameraControl.so
    ${OpenCV_LIBS}
    )
]]