#pragma once
#include <iostream>

#include <opencv2/opencv.hpp>

#include <MvCameraControl.h>

class Camera
{
public:
    Camera(); 
    Camera(MV_CC_DEVICE_INFO_LIST list, int n); 

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    ~Camera();

public:
    int create_handle(MV_CC_DEVICE_INFO_LIST list, int n);
    int open_device();
    int start_grabbing();
    

    MV_FRAME_OUT get_image();
    int stop_grabbing();
    int close_device();


private:
    void* m_handle = nullptr;

    bool m_opened = false;
    bool m_grabbing = false;


};