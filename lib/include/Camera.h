#pragma once
#include <iostream>

#include <opencv2/opencv.hpp>

#include <MvCameraControl.h>

class Camera
{
public:
    static int cc_initailize();
    static int cc_enum_devices(MV_CC_DEVICE_INFO_LIST& list);
    static int cc_finalize();

private:
    static void PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo);
    static bool initialized;

public:
    Camera(); 

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    ~Camera();

    int create_handle(MV_CC_DEVICE_INFO_LIST& list, int n);
    int open_device();
    int start_grabbing();
    int get_image(cv::Mat& img);

    int get_exposure_time();
    int get_auto_exposure();
    int get_gain_value();


    int set_exposure_time(float fExposureTime);
    int set_auto_exposure(unsigned int nExposureAuto);
    int set_gain_value(float fGain);

    int stop_grabbing();
    int close_device();

private:
    void* m_handle = nullptr;

    bool m_opened = false;
    bool m_grabbing = false;


};