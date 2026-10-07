#include <Camera.h>

Camera::Camera(){};

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


int Camera::create_handle(MV_CC_DEVICE_INFO_LIST& list, int n)
{
    if (0 <= n && 
        n < list.nDeviceNum && 
        list.pDeviceInfo[n] != nullptr &&
        m_handle == nullptr)   
    {
        void* new_handle = nullptr;
        int nRet = MV_CC_CreateHandle(&new_handle, list.pDeviceInfo[n]);

        if (nRet == MV_OK)
        {
            m_handle = new_handle;
            new_handle = nullptr;
            std::cout << "CreateHandle succeed! " << std::endl; 
        }
        else
        {
            std::cout << "CreateHandle fail  " << nRet << std::endl; 
        }

        return nRet;
    }
    else
    {
        std::cout << "CreateHandle fail  "  << std::endl; 
        return -1;
    }
    
}


int Camera::open_device()
{
    if (m_opened == false && m_handle != nullptr)
    {
        int nRet = MV_CC_OpenDevice(m_handle);

        if (nRet != MV_OK)
        {
            std::cout << "OpenDevice fail!  " << nRet << std::endl;
            //如果失败，相机处于关闭状态，后面的接口无法通过调用条件.但句柄依旧存在，允许再次尝试打开
        }

        else
        {
            std::cout << "OpenDevice succeed!" << std::endl;
            m_opened = true;
        }
        return nRet;
    }
    else
    {
        std::cout << "OpenDevice fail!" << std::endl;
        return -1;
    }
    
}


int Camera::start_grabbing()
{
    if (m_opened == true && m_handle != nullptr && m_grabbing == false)
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
    else
    {
        std::cout << "StartGrabbing fail!" << std::endl;
        return -1;
    }

}

int Camera::get_exposure_time()
{
    if (m_handle != nullptr && m_opened == true)
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

}

int Camera::get_auto_exposure()
{
    if (m_handle != nullptr && m_opened == true)
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

}

int Camera::get_gain_value()
{
    if (m_handle != nullptr && m_opened == true)
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

}

int Camera::set_exposure_time(float fExposureTime)
{
    if (m_handle != nullptr && m_opened == true)
    {
        MVCC_FLOATVALUE stExposureTime = {0};
        int nRet = MV_CC_GetFloatValue(m_handle, "ExposureTime", &stExposureTime);

        if (MV_OK == nRet)
        { 
            if (stExposureTime.fMin <= fExposureTime && fExposureTime <= stExposureTime.fMin)
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
            }
        }

        else
        {
            std::cout << "set exposure time failed!" << std::endl;
        }

        return nRet;
    }
    else
    {
        std::cout << "set exposure time failed!" << std::endl;
        return -1;
    }
    
}

int Camera::set_auto_exposure(unsigned int nExposureAuto)
{
    if (m_handle != nullptr && m_opened == true)
    {
        MVCC_ENUMVALUE stExposureAuto = {0};
        int nRet = MV_CC_GetEnumValue(m_handle, "ExposureAuto", &stExposureAuto);

        if (MV_OK == nRet)
        {
            if (0 <= nExposureAuto && nExposureAuto < stExposureAuto.nSupportedNum)
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
            }
            else
            {
                std::cout << "set ExposureAuto failed!" << std::endl;
            }
        }
        else
        {
            std::cout << "set ExposureAuto failed!" << std::endl;
        }
        
        return nRet;
    }

}

int Camera::set_gain_value(float fGain)
{
    if (m_handle != nullptr && m_opened == true)
    {
        MVCC_FLOATVALUE stGain = {0};
        int nRet = MV_CC_GetFloatValue(m_handle, "Gain", &stGain);
        if (MV_OK == nRet)
        {
            if (stGain.fMin <= fGain && fGain <= stGain.fMax)
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
            }
        }
        else
        {
            std::cout << "set Gain failed!" << std::endl;
        }
        return nRet;
    }

    else
    {
        std::cout << "set Gain failed!" << std::endl;
        return -1;
    }

  
}

int Camera::get_image(cv::Mat& img)//待处理：如果img不是那个大小呢？如果不是bgr呢？所以真的要输入一个img吗？
{
    if (m_handle != nullptr && m_grabbing == true)//问题：如果出现失败的情况呢？ 
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

}

int Camera::stop_grabbing()
{
    if (m_handle != nullptr && m_grabbing)
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
    else
    {
        std::cout << "Stop grabbing fail!" << std::endl;
        return -1;
    }

}


int Camera::close_device()
{
    if (m_handle != nullptr && m_opened == true && m_grabbing != true)
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
    else
    {
        std::cout << "CloseDevice failed!" << std::endl;
        return -1;
        //可以再细化，但暂时不做，比如输出对应的原因
    }
}






