#include <Camera.h>
#include <SdkCtrl.h>

int initailize_and_enum(MV_CC_DEVICE_INFO_LIST& LIST)
{
    int OK = MV_OK;

    OK = cc_initailize();
    if(OK != MV_OK) {return OK;}

    OK = cc_enum_devices(LIST);
    if(OK != MV_OK) {return OK;}

    return OK;
}

int camera_test(Camera& cam)
{
    int OK = MV_OK;
    cv::Mat img;

    OK = cam.open_device();
    if(OK != MV_OK) {return OK;}

    OK = cam.start_grabbing();
    if(OK != MV_OK) {return OK;}

    while(true)
    {
        OK = cam.get_image(img);
        
        cv::imshow("image", img);
        int key = cv::waitKey(1);

        if(key == 'q') {break;}
    }

    OK = cam.stop_grabbing();
    if(OK != MV_OK) {return OK;}

    OK = cam.close_device();
    if(OK != MV_OK) {return OK;}

    return OK;
}

int main()
{
    int OK = MV_OK;
    MV_CC_DEVICE_INFO_LIST stDeviceList;

    OK = initailize_and_enum(stDeviceList);
    
    if(OK == MV_OK)
    {
        Camera cam1(stDeviceList);
        // cam1.create_handle(stDeviceList);
        OK = camera_test(cam1);
    }
    
    cc_finalize();
    return OK;
}

