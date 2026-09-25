#生成动态库
add_library(
    Camera 
    SHARED 
    src/Camera.cpp)

target_include_directories(
    Camera 
    PUBLIC 
    include
    /opt/MVS/include
    )

target_link_libraries(
    Camera 
    PUBLIC
    /opt/MVS/lib/64/libMvCameraControl.so
    ${OpenCV_LIBS}
    )
