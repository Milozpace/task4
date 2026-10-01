#include <Camera.h>

Camera::Camera() {}

Camera::Camera(MV_CC_DEVICE_INFO_LIST list, int n)
{
    create_handle(list, n);
}


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


int Camera::create_handle(MV_CC_DEVICE_INFO_LIST list, int n)
{
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


//展示图片
MV_FRAME_OUT Camera::get_image()
{
    while(true)
    {
        MV_FRAME_OUT frame = {0};
        int nRet = MV_CC_GetImageBuffer(m_handle,&frame, 1000);

        if(nRet != MV_OK)
        {
            std::cout << "GetImage fail!" << std::endl;
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
            
            MV_CC_FreeImageBuffer(m_handle, &frame);

            cv::imshow("bgr", bgr);
            int key = cv::waitKey(1);
            if(key == 'q' || key == 27) {break;}
        }
        return frame;
    }
    
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





