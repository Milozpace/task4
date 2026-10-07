#include <Camera.h>

bool Camera::initialized = false;

int Camera::cc_initailize()
{
    int nRet = MV_CC_Initialize();
    if(nRet != MV_OK)
    {
        std::cout << "Initailize fail  " << nRet << std::endl;
    }
    else
    {
        Camera::initialized = true;
        std::cout << "Initailize succeed!" << std::endl;
    }

    return nRet;
}

int Camera::cc_enum_devices(MV_CC_DEVICE_INFO_LIST& list)
{
    if (Camera::initialized == true)
    {
        int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE | MV_GENTL_CAMERALINK_DEVICE | MV_GENTL_CXP_DEVICE | MV_GENTL_XOF_DEVICE | MV_GENTL_XOC_DEVICE, &list);
        
        if(nRet != MV_OK)
        {
            std::cout << "EnumDevices fail  " << nRet << std::endl;
        }
        if (list.nDeviceNum > 0)
        {
            for (int i = 0; i < list.nDeviceNum; i++)
            {
                printf("[device %d]:\n", i);
                MV_CC_DEVICE_INFO* pDeviceInfo = list.pDeviceInfo[i];
                if (NULL == pDeviceInfo)
                {
                    break;
                }
                PrintDeviceInfo(pDeviceInfo);            
            }    
        } 
        else
        {
            std::cout << "Find No Devices!" << std::endl;
        }

        return nRet;
    }
    else
    {
        std::cout << "Please initialize first" << std::endl;
        return -1;
    }
}

int Camera::cc_finalize()
{
    if (Camera::initialized == true)
    {
        int nRet = MV_CC_Finalize();
        if(nRet != MV_OK)
        {
            std::cout << "Finalize fail  " << nRet << std::endl;
        }
        else
        {
            std::cout << "Finalize succeed!" << std::endl;
        }

        return nRet;
    }
    else
    {
        std::cout << "You didn't initalize" << std::endl;
        return -1;
    }
}

void Camera::PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo)
{
    if (NULL == pstMVDevInfo)
    {
        printf("The Pointer of pstMVDevInfo is NULL!\n");
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

}

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
    if (m_opened == true && m_grabbing == false)
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
    if (m_opened == true)
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
    else
    {
        std::cout << "get ExposureTime failed!" << std::endl;
        return -1;
    }

}

int Camera::get_auto_exposure()
{
    if (m_opened == true)
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
    else
    {
        std::cout << "set auto exposure fail!" << std::endl;
        return -1;
    }

}

int Camera::get_gain_value()
{
    if (m_opened == true)
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
    else
    {
        std::cout << "get Gain failed!" << std::endl;
        return -1;
    }

}

int Camera::set_exposure_time(float fExposureTime)
{
    if (m_opened == true)
    {
        MVCC_FLOATVALUE stExposureTime = {0};
        int nRet = MV_CC_GetFloatValue(m_handle, "ExposureTime", &stExposureTime);

        if (MV_OK == nRet)
        { 
            if (stExposureTime.fMin <= fExposureTime && fExposureTime <= stExposureTime.fMax)
            {
                nRet = MV_CC_SetFloatValue(m_handle, "ExposureTime", fExposureTime);
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
    if (m_opened == true)
    {
        MVCC_ENUMVALUE stExposureAuto = {0};
        int nRet = MV_CC_GetEnumValue(m_handle, "ExposureAuto", &stExposureAuto);

        if (MV_OK == nRet)
        {
            if (nExposureAuto < stExposureAuto.nSupportedNum)
            {
                nRet = MV_CC_SetEnumValue(m_handle, "ExposureAuto", nExposureAuto);
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
    if (m_opened == true)
    {
        MVCC_FLOATVALUE stGain = {0};
        int nRet = MV_CC_GetFloatValue(m_handle, "Gain", &stGain);
        if (MV_OK == nRet)
        {
            if (stGain.fMin <= fGain && fGain <= stGain.fMax)
            {
                nRet = MV_CC_SetFloatValue(m_handle, "Gain", fGain);
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

int Camera::get_image(cv::Mat& img)
{
    if (m_grabbing == true)
    {
        MV_FRAME_OUT frame = {0};
        int nRet = MV_CC_GetImageBuffer(m_handle,&frame, 1000);

        if(nRet != MV_OK)
        {
            img.release();
            std::cout << "GetImage fail!  " << nRet << std::endl;
            return nRet;
        }
        else
        {
            cv::Mat raw(frame.stFrameInfo.nHeight,
                        frame.stFrameInfo.nWidth,
                        CV_8UC1,
                        frame.pBufAddr);
            bool cvt = false;
            try
            {
                cv::cvtColor(raw, img, cv::COLOR_BayerRGGB2BGR);
                cvt = true;
            }
            catch (const cv::Exception& e)
            {
                std::cout << "cvtColor failed: " << e.what() << std::endl;
            }

            int nRet = MV_CC_FreeImageBuffer(m_handle, &frame);
            if (nRet != MV_OK)
            {
                std::cout << "Free Image fail: " << nRet << std::endl;
            }

            if (cvt == false)
            {
                return -1;
            }

            return nRet;
        }


    }
    else
    {
        std::cout << "GetImage fail!" << std::endl;
        return -1;
    }

}

int Camera::stop_grabbing()
{
    if (m_grabbing == true)
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
    if (m_opened == true)
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






