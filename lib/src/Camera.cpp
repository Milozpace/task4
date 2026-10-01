#include <Camera.h>

Camera::Camera(){};

Camera::Camera(MV_CC_DEVICE_INFO_LIST& list)
{
    create_handle(list);
}

Camera::~Camera()
{
    if(m_handle != nullptr)
    {
        if(m_grabbing == true)
        {
            stop_grabbing();
        }

        if(m_opened == true)
        {
            close_device();
        }
        
        int nRet = MV_CC_DestroyHandle(m_handle);
        m_handle = nullptr;
        std::cout << "destroy handle" << std::endl;
    }

}


int Camera::create_handle(MV_CC_DEVICE_INFO_LIST& list)
{
    int nIndex;
    if(list.nDeviceNum > 0)
    {
        std::cout << "choose your camera index: " << std::endl;
        std::cin >> nIndex;
        if(nIndex >= list.nDeviceNum)
        {
            std::cout << "Intput error!" << std::endl;
            return -1;
        }
    }

    int nRet = MV_CC_CreateHandle(&m_handle, list.pDeviceInfo[nIndex]);

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

int Camera::get_exposure_time()
{
    MVCC_FLOATVALUE stExposureTime = {0};
    int nRet = MV_CC_GetFloatValue(m_handle, "ExposureTime", &stExposureTime);

    if(nRet != MV_OK)
    {
        std::cout << "get ExposureTime failed!" << std::endl;
    }
    else
    {
        std::cout << "exposure time current value:" << stExposureTime.fCurValue << std::endl;
        std::cout << "exposure time max value:" << stExposureTime.fMax << std::endl;
        std::cout << "exposure time min value:" << stExposureTime.fMin << std::endl;
    }
    return nRet;
}

int Camera::get_auto_exposure()
{
    MVCC_ENUMVALUE stExposureAuto = {0};
    int nRet = MV_CC_GetEnumValue(m_handle, "ExposureAuto", &stExposureAuto) ;

    if(nRet != MV_OK)
    {
        std::cout << "set auto exposure fail!" << std::endl;
    }
    else
    {
        std::cout << "ExposureAuto current value:" << stExposureAuto.nCurValue << std::endl;
        std::cout << "supported ExposureAuto number:" << stExposureAuto.nSupportedNum << std::endl;
        for(int i = 0; i < stExposureAuto.nSupportedNum; i++)
        {
            std::cout << "supported ExposureAuto:" << stExposureAuto.nSupportValue[i] << std::endl;
        }
    }

    return nRet;
}

int Camera::get_gain_value()
{
    MVCC_FLOATVALUE stGain = {0};
    int nRet = MV_CC_GetFloatValue(m_handle, "Gain", &stGain);

    if(nRet != MV_OK)
    {
        std::cout << "get Gain failed!" << std::endl;
    }
    else
    {
        std::cout << "gain current value:" << stGain.fCurValue << std::endl;
        std::cout << "gain max value:" << stGain.fMax << std::endl;
        std::cout << "gain min value:" << stGain.fMin << std::endl;
    }
    return nRet;
}

int Camera::set_exposure_time(float fExposureTime)
{
    int nRet = MV_CC_SetFloatValue(m_handle, "ExposureTime", fExposureTime);
    if (MV_OK == nRet)
    {
        std::cout << "set exposure time OK!" << std::endl;
    }
    else
    {
        std::cout << "set exposure time failed!" << std::endl;
    }
    return nRet;
}

int Camera::set_auto_exposure(unsigned int nExposureAuto)
{
    int nRet = MV_CC_SetEnumValue(m_handle, "ExposureAuto", nExposureAuto);
    if (MV_OK == nRet)
    {
        std::cout << "set ExposureAuto OK!" << std::endl;
    }
    else
    {
        std::cout << "set ExposureAuto failed!" << std::endl;
    }
    return nRet;
}

int Camera::set_gain_value(float fGain)
{
    int nRet = MV_CC_SetFloatValue(m_handle, "Gain", fGain);
    if (MV_OK == nRet)
    {
        std::cout << "set Gain OK!" << std::endl;
    }
    else
    {
        std::cout << "set Gain failed!" << std::endl;
    }
    return nRet;
}

int Camera::get_image(cv::Mat& img)
{
    MV_FRAME_OUT frame = {0};
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





