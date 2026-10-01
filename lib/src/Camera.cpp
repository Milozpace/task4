#include <Camera.h>

Camera::Camera(){};

// Camera::Camera(MV_CC_DEVICE_INFO_LIST& list)
// {
//     create_handle(list);
// }

Camera::~Camera()
{
    if(m_handle != nullptr)
    {
        if(m_grabbing == true)
        {
            stop_grabbing();
            std::cout << "stop grabbing" << std::endl;
        }

        if(m_opened == true)
        {
            close_device();
            std::cout << "camera has closed" << std::endl;
        }
        
        int nRet = MV_CC_DestroyHandle(m_handle);
        m_handle = nullptr;
        std::cout << "destroy handle" << std::endl;
    }

}


int Camera::create_handle(MV_CC_DEVICE_INFO_LIST& list)
{
    std::cout << "choose your camera index: " << std::endl;
    int n;
    std::cin >> n;
    if(n >= list.nDeviceNum)
    {
        std::cout << "Intput error!" << std::endl;
        return -1;
    }

    int nRet = MV_CC_CreateHandle(&m_handle, list.pDeviceInfo[n]);

    if (nRet != MV_OK)
    {
        std::cout << "CreateHandle fail  " << nRet << std::endl; 
    }
    else
    {
        std::cout << "CreateHandle succeed! " << std::endl; 
    }

    return nRet;
}


int Camera::open_device()
{
    int nRet = MV_CC_OpenDevice(m_handle);

    if (nRet != MV_OK)
    {
        std::cout << "OpenDevice fail!  " << nRet << std::endl;
    }
    else
    {
        std::cout << "OpenDevice succeed!" << std::endl;
        m_opened = true;
    }
    return nRet;
}


int Camera::start_grabbing()
{
    int nRet = MV_CC_StartGrabbing(m_handle);
    if(nRet != MV_OK)
    {
        std::cout << "StartGrabbing fail!  " << nRet << std::endl;
    }
    else
    {
        std::cout << "StartGrabbing succeed!" << std::endl;
        m_grabbing = true;
    }
    return nRet;
}


int Camera::get_image(cv::Mat& img)
{
    MV_FRAME_OUT frame = {};
    int nRet = MV_CC_GetImageBuffer(m_handle,&frame, 1000);

    if(nRet != MV_OK)
    {
        std::cout << "GetImage fail!  " << nRet << std::endl;
    }
    else
    {
        cv::Mat raw(frame.stFrameInfo.nHeight,
                    frame.stFrameInfo.nWidth,
                    CV_8UC1,
                    frame.pBufAddr);

        cv::cvtColor(raw, img, cv::COLOR_BayerRGGB2BGR);
        
        MV_CC_FreeImageBuffer(m_handle, &frame);
    }
    return nRet;
}

int Camera::stop_grabbing()
{
    int nRet = MV_CC_StopGrabbing(m_handle);
    if(nRet != MV_OK)
    {
        std::cout << "Stop grabbing fail!" << std::endl;
    }
    else
    {
        std::cout << "Stop grabbing succeed!" << std::endl;
        m_grabbing = false;
    }
    return nRet;
}


int Camera::close_device()
{
    int nRet = MV_CC_CloseDevice(m_handle);
    
    if(nRet != MV_OK)
    {
        std::cout << "CloseDevice failed!" << std::endl;
    }
    else
    {
        std::cout << "CloseDevice succeed!" << std::endl;
        m_opened = false;
    }
    return nRet;
}





