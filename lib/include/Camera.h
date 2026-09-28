#pragma once
#include <iostream>

#include <opencv2/opencv.hpp>

#include <MvCameraControl.h>

class Camera
{
public:
    Camera(); //构建函数

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;//禁止复制（改进）

    ~Camera();//析构

private:
    static bool initalize();//初始化SDK

    static bool enum_camera();//枚举相机(并打印)

    bool create_handle();//创造句柄

    bool open_camera();//打开相机

    bool start_grabbing();//开始取流

    bool stop_grabbing();//停止取流

    bool close_camera();//关闭相机

public:
    static bool set_up();//启动：完成初始化并枚举设备

    static void search_camera();//寻找设备：枚举（但不初始化）

    bool run_camera();//运行相机

    bool get_image();//取图

    bool stop_run_camera();//停止相机运行

    static bool finalize();//反初始化

private:
    int nRet = MV_OK;
    void* handle = nullptr;

    //状态跟踪
    static bool initalized;
    bool opened = false;
    bool grabbing = false;

    //设备列表
    static MV_CC_DEVICE_INFO_LIST m_device_list;

};