#include <Camera.h>

//static变量定义
bool Camera::initalized = false;
MV_CC_DEVICE_INFO_LIST Camera::m_device_list{};

//打印设备信息函数（c）
bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo)
{
    if (NULL == pstMVDevInfo)
    {
        printf("The Pointer of pstMVDevInfo is NULL!\n");
        return false;
    }
    if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE)
    {
        int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
        int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
        int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
        int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

        // ch:打印当前相机ip和用户自定义名字 | en:print current ip and user defined name
        printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName);
        printf("CurrentIp: %d.%d.%d.%d\n" , nIp1, nIp2, nIp3, nIp4);
        printf("UserDefinedName: %s\n\n" , pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
    }
    else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
    {
        printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chModelName);
        printf("UserDefinedName: %s\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
    }
    else if (pstMVDevInfo->nTLayerType == MV_GENTL_GIGE_DEVICE)
    {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chSerialNumber);
        printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName);
    }
    else if (pstMVDevInfo->nTLayerType == MV_GENTL_CAMERALINK_DEVICE)
    {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stCMLInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stCMLInfo.chSerialNumber);
        printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stCMLInfo.chModelName);
    }
    else if (pstMVDevInfo->nTLayerType == MV_GENTL_CXP_DEVICE)
    {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stCXPInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stCXPInfo.chSerialNumber);
        printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stCXPInfo.chModelName);
    }
    else if (pstMVDevInfo->nTLayerType == MV_GENTL_XOF_DEVICE)
    {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stXoFInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stXoFInfo.chSerialNumber);
        printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stXoFInfo.chModelName);
    }
    else if (pstMVDevInfo->nTLayerType == MV_GENTL_XOC_DEVICE)
    {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stXoCInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stXoCInfo.chSerialNumber);
        printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stXoCInfo.chModelName);
    }
    else
    {
        printf("Not support.\n");
    }

    return true;
}


Camera::Camera()
{
    std::cout << "Camera created!" << std::endl;
}

// 初始化SDK
bool Camera::initalize()
{
    if(MV_CC_Initialize() == MV_OK)
    {
        std::cout << "Initialize Succeed!" << std::endl;
        initalized = true;
        return true;
    }
    else
    {
        std::cout << "Initialize fail!" << std::endl;
        return false;
    }
}

// 枚举设备
bool Camera::enum_camera()
{
    int n = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE | MV_GENTL_CAMERALINK_DEVICE | MV_GENTL_CXP_DEVICE | MV_GENTL_XOF_DEVICE | MV_GENTL_XOC_DEVICE, &m_device_list);
        if (n != MV_OK)
        {
            printf("EnumDevices fail! nRet [%x]\n", n);
            return false;
        }

        if (m_device_list.nDeviceNum > 0)
        {
            for (int i = 0; i < m_device_list.nDeviceNum; i++)
            {
                printf("[device %d]:\n", i);
                MV_CC_DEVICE_INFO* pDeviceInfo = m_device_list.pDeviceInfo[i];
                if (NULL == pDeviceInfo)
                {
                    return false;
                } 
                PrintDeviceInfo(pDeviceInfo);    
                      
            }  
            return true;  
        } 
        else
        {
            printf("Find No Devices!\n");
            return false;
        }

}

// 创建句柄
bool Camera::create_handle()
{
    //输入编号并选择
    std::cout << "Please Intput camera index: " ;
    unsigned int nIndex = 0;
    std::cin >> nIndex;
    std::cout << std::endl;

    if(nIndex >= m_device_list.nDeviceNum)
    {
        std::cout << "Intput error!" << std::endl;
        return false;
    }

    //创造句柄
    nRet = MV_CC_CreateHandle(&handle, m_device_list.pDeviceInfo[nIndex]);

    if (nRet != MV_OK)
    {
        std::cout << "CreateHandle fail! " << std::endl; 
        return false;
    }
    else
    {
        std::cout << "CreateHandle succeed! " << std::endl; 
        return true;
    }
}

// 打开设备
bool Camera::open_camera()
{
    nRet = MV_CC_OpenDevice(handle);

    if (nRet != MV_OK)
    {
        std::cout << "OpenDevice fail!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "OpenDevice succeed!" << std::endl;
        opened = true;
        return true;
    }
}

// 设置node&开始取流
bool Camera::start_grabbing()
{
    nRet = MV_CC_SetImageNodeNum(handle, 5);
    
    if(nRet != MV_OK)
    {
        std::cout << "SetImageNodeNum fail" << std::endl;
        std::cout << "  Node: 1" << std::endl;
    }
    else
    {
        std::cout << "SetImageNodeNum succeed" << std::endl;
        std::cout << "  Node: 5" << std::endl;
    }

    nRet = MV_CC_StartGrabbing(handle);
    if(nRet != MV_OK)
    {
        std::cout << "StartGrabbing fail!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "StartGrabbing succeed!" << std::endl;
        grabbing = true;

        return true;
    }
}


//展示图片
bool Camera::get_image()
{
    while(true)
    {
        MV_FRAME_OUT frame = {0};
        nRet = MV_CC_GetImageBuffer(handle,&frame, 1000);

        if(nRet != MV_OK)
        {
            std::cout << "GetImage fail!" << std::endl;
            return false;
            break;
        }
        else
        {
            std::cout << "width:" << frame.stFrameInfo.nWidth << " ";
            std::cout << "height" << frame.stFrameInfo.nHeight << " ";
            std::cout << "frame:" << frame.stFrameInfo.nFrameNum << "  ";
            std::cout << std::endl;

            cv::Mat raw(frame.stFrameInfo.nHeight,
                        frame.stFrameInfo.nWidth,
                        CV_8UC1,
                        frame.pBufAddr);

            cv::Mat bgr;
            cv::cvtColor(raw, bgr, cv::COLOR_BayerRGGB2BGR);
            
            MV_CC_FreeImageBuffer(handle, &frame);

            cv::imshow("bgr", bgr);
            int key = cv::waitKey(1);
            if(key == 'q' || key == 27) {break;};
        }

    }
    return true;
}


// 停止取流
bool Camera::stop_grabbing()
{
    nRet = MV_CC_StopGrabbing(handle);
    if(nRet != MV_OK)
    {
        std::cout << "Stop grabbing fail!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Stop grabbing succeed!" << std::endl;
        grabbing = false;
        return true;
    }
}

// 关闭设备
bool Camera::close_camera()
{
    nRet = MV_CC_CloseDevice(handle);
    
    if(nRet != MV_OK)
    {
        std::cout << "CloseDevice failed!" << std::endl;
        return false;
    }

    else
    {
        std::cout << "CloseDevice succeed!" << std::endl;
        opened = false;
        return true;
    }
}

//反初始化
bool Camera::finalize()
{
    int result = MV_CC_Finalize();

    if(result != MV_OK)
    {
        std::cout << "finalize fail!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "exit" << std::endl;
        initalized = false;
        return true;
    }
}

// 析构函数
Camera::~Camera()
{
    if(handle != nullptr)
    {
        if(grabbing == true)
        {
            stop_grabbing();
            std::cout << "stop grabbing" << std::endl;
        }

        if(opened == true)
        {
            close_camera();
            std::cout << "camera has closed" << std::endl;
        }
        
        nRet = MV_CC_DestroyHandle(handle);
        handle = nullptr;
        std::cout << "destroy handle" << std::endl;
    }

}


///以下为可调用接口


//启动并枚举
bool Camera::set_up()
{
    if(Camera::initalize() != true) return false;
    if(Camera::enum_camera() != true) return false;
    
    return true;
}

//仅枚举（添加运行设备）
void Camera::search_camera()
{
    if(initalized != true)
    {
        std::cout << "Search camera fail!" << " You haven't initalize." << std::endl;
    }
    else
    {
        Camera::enum_camera();
    }
}

//运行相机
bool Camera::run_camera()
{
    if(create_handle() != true) return false;
    if(open_camera() != true) return false;
    if(start_grabbing() != true) return false;

    return true;
}


//停止取流并关闭相机
bool Camera::stop_run_camera()
{
    if(stop_grabbing() != true) return false;
    if(close_camera() != true) return false;

    return true;
}


//END


