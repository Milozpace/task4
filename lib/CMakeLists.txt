#Camera库
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

#SdkCtrl库
add_library(
    SdkCtrl 
    SHARED 
    src/SdkCtrl.cpp)

target_include_directories(
    SdkCtrl 
    PUBLIC 
    include
    /opt/MVS/include
    )

target_link_libraries(
    SdkCtrl 
    PUBLIC
    /opt/MVS/lib/64/libMvCameraControl.so
    )