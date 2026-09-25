#include <iostream>
#include "MvCameraControl.h"
#include <string>

bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo)
{
    if (NULL == pstMVDevInfo)
    {
        printf("The Pointer of pstMVDevInfo is NULL!\n");
        return false;
    }
    else
    {
    printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chModelName);
    printf("UserDefinedName: %s\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
    }

    return true;
}

int check(int ret,std::string name)
{
    if(ret == MV_OK)
    {
        std::cout << name << ":ok" << std::endl;
        return 1;
    }

    else
    {
        std::cout << name << ":nonono" << std::endl;
        return -1;
    }

}

bool running = true;
int count = 0;


int main()
{
    //初始化
    int nRet = MV_CC_Initialize();

    //枚举相机并打印信息
    MV_CC_DEVICE_INFO_LIST Device{};

    nRet = MV_CC_EnumDevices(MV_USB_DEVICE, &Device);
    check(nRet, "Enum");
    if(Device.nDeviceNum > 0)
    {
        MV_CC_DEVICE_INFO* device = Device.pDeviceInfo[0];
        
        PrintDeviceInfo(device);
    }

    //创造实例
    void* handle = NULL;
    nRet = MV_CC_CreateHandle(&handle, Device.pDeviceInfo[0]);
    check(nRet, "handle create");

    //打开相机
    nRet = MV_CC_OpenDevice(handle);
    check(nRet, "open camera");
    
    //取图
    MV_CC_SetImageNodeNum(handle, 5);
    MV_CC_StartGrabbing(handle);

    while(running)
    {
        MV_FRAME_OUT frame = {0};

        nRet = MV_CC_GetImageBuffer(handle,&frame, 1000);
        check(nRet, "Getimage");
        
        if(nRet == MV_OK)
        {
            std::cout << "width:" << frame.stFrameInfo.nWidth << " ";
            std::cout << "height" << frame.stFrameInfo.nHeight << " ";
            std::cout << "frame:" << frame.stFrameInfo.nFrameNum << std::endl;

            MV_CC_FreeImageBuffer(handle, &frame);

            count++;
        }
        else
        {
            std::cout << "grabbing error" << std::endl;
        }

        if(count >= 100)
        {
            running = false;
        }
    }

    //停止取流
    MV_CC_StopGrabbing(handle);

    //关闭相机
    nRet = MV_CC_CloseDevice(handle);
    check(nRet,"close camera");

    //销毁实例
    nRet = MV_CC_DestroyHandle(handle);
    check(nRet, "destroy handle");


    MV_CC_Finalize();

    return nRet;
}
