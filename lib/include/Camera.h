#pragma once
#include <iostream>

#include <memory>

#include <opencv2/opencv.hpp>

#include <MvCameraControl.h>

class Camera
{
public:
    Camera(); //构建函数

    static int init();//初始化SDK

    int enum_camera();//枚举相机(并打印)

    int create_handle();//创造句柄

    int open_camera();//打开相机

    int start_grabbing();//开始取流

    
    int show_image();//获取并展示图片


    int stop_grabbing();//停止取流

    int close_camera();//关闭相机

    static void finalize();

    ~Camera();//析构
public:
    static int run_camera();

private:
    static int nRet;
    void* handle;


    MV_CC_DEVICE_INFO_LIST m_device_list{};//ps:写不写这个{}有什么区别

};