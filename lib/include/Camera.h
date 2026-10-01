#pragma once
#include <iostream>

#include <opencv2/opencv.hpp>

#include <MvCameraControl.h>

class Camera
{
public:
    Camera(); 
    //Camera(MV_CC_DEVICE_INFO_LIST& list); 

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    ~Camera();

public:
    int create_handle(MV_CC_DEVICE_INFO_LIST& list);
    int open_device();
    int start_grabbing();
    

    int get_image(cv::Mat& img);
    int stop_grabbing();
    int close_device();


private:
    void* m_handle = nullptr;

    bool m_opened = false;
    bool m_grabbing = false;


};