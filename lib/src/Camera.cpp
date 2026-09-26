#include <Camera.h>

bool Camera::initalized = false;
MV_CC_DEVICE_INFO_LIST Camera::m_device_list{};

//复制
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
    : nRet(MV_OK)
{
    std::cout << "Hello" << std::endl;
}

// 初始化SDK
int Camera::initalize()
{
    if(MV_CC_Initialize() == MV_OK)
    {
        std::cout << "Initialize Succeed!" << std::endl;
        initalized = true;
        return 1;
    }
    else
    {
        std::cout << "Initialize fail!" << std::endl;
        return -1;
    }
}

// 枚举设备
int Camera::enum_camera()
{
    int n = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE | MV_GENTL_CAMERALINK_DEVICE | MV_GENTL_CXP_DEVICE | MV_GENTL_XOF_DEVICE | MV_GENTL_XOC_DEVICE, &m_device_list);
        if (n != MV_OK)
        {
            printf("EnumDevices fail! nRet [%x]\n", n);
            return -1;
        }

        if (m_device_list.nDeviceNum > 0)
        {
            for (int i = 0; i < m_device_list.nDeviceNum; i++)
            {
                printf("[device %d]:\n", i);
                MV_CC_DEVICE_INFO* pDeviceInfo = m_device_list.pDeviceInfo[i];
                if (NULL == pDeviceInfo)
                {
                    return -1;
                } 
                PrintDeviceInfo(pDeviceInfo);    
                      
            }  
            return 1;  
        } 
        else
        {
            printf("Find No Devices!\n");
            return -1;
        }

}

// 创建句柄
int Camera::create_handle()
{
    //输入编号并选择
    std::cout << "Please Intput camera index: " ;
    unsigned int nIndex = 0;
    std::cin >> nIndex;
    std::cout << std::endl;

    if(nIndex >= m_device_list.nDeviceNum)
    {
        std::cout << "Intput error!" << std::endl;
        return -1;
    }

    //创造
    nRet = MV_CC_CreateHandle(&handle, m_device_list.pDeviceInfo[nIndex]);

    if (nRet != MV_OK)
    {
        std::cout << "CreateHandle fail! " << std::endl; 
        return -1;
    }
    else
    {
        std::cout << "CreateHandle succeed! " << std::endl; 
        return 1;
    }
}

// 打开设备
int Camera::open_camera()
{
    nRet = MV_CC_OpenDevice(handle);

    if (nRet != MV_OK)
    {
        std::cout << "OpenDevice fail!" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "OpenDevice succeed!" << std::endl;
        opened = true;
        return 1;
    }
}

// 设置线程&开始取流
int Camera::start_grabbing()
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
        return -1;
    }
    else
    {
        std::cout << "StartGrabbing succeed!" << std::endl;
        grabbing = true;
        running = true;

        return 1;
    }
}


//展示图片
int Camera::show_image()
{
    int n;
    std::cout << "Input GetImage Number:" ;
    std::cin >> n;
    std::cout << std::endl;

    while(running)
    {
    MV_FRAME_OUT frame = {0};
    nRet = MV_CC_GetImageBuffer(handle,&frame, 1000);

    if(nRet != MV_OK)
    {
        std::cout << "GetImage fail!" << std::endl;
        return -1;
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
        cv::waitKey(1);

        count++;
    }

    if(count >= n)
    {
        running = false;
    }
    
    }
    return 1;
}


// 停止取流
int Camera::stop_grabbing()
{
    nRet = MV_CC_StopGrabbing(handle);
    if(nRet != MV_OK)
    {
        std::cout << "Stop grabbing fail!" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "Stop grabbing succeed!" << std::endl;
        grabbing = false;
        return 1;
    }
}

// 关闭设备
int Camera::close_camera()
{
    nRet = MV_CC_CloseDevice(handle);
    
    if(nRet != MV_OK)
    {
        std::cout << "CloseDevice failed!" << std::endl;
        return -1;
    }

    else
    {
        std::cout << "CloseDevice succeed!" << std::endl;
        opened = false;
        return 1;
    }
}

//反初始化
int Camera::finalize()
{
    int result = MV_CC_Finalize();

    if(result != MV_OK)
    {
        std::cout << "finalize fail!" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "exit" << std::endl;
        initalized = false;
        return 1;
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

//////以下为可调用接口///////

//启动并枚举
int Camera::set_up()
{
    if(Camera::initalize() != 1) return -1;
    if(Camera::enum_camera() != 1) return -1;
    
    return 1;
}

//仅枚举（添加运行设备）
void Camera::search_camera()
{
    std::cout << initalized << std::endl;
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
int Camera::run_camera()
{
    if(create_handle() != 1) return -1;
    if(open_camera() != 1) return -1;

    return 1;
}

//取图
int Camera::capture_image()
{
    if(start_grabbing() != 1) return -1;

    if(show_image() != 1) return -1;

    if(stop_grabbing() != 1) return -1;

    return 1;
}

//关闭相机的定义在上面


