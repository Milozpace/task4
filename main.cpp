#include <Camera.h>

int main()
{
    int OK = MV_OK;
    MV_CC_DEVICE_INFO_LIST stDeviceList;

    OK = Camera::cc_initailize();

    if(OK == MV_OK) {OK = Camera::cc_enum_devices(stDeviceList);}
    
    if(OK == MV_OK)
    {
        Camera cam;
        OK = cam.create_handle(stDeviceList, 0); 
        cv::Mat img;

        if(OK == MV_OK) {OK = cam.open_device();}
        if(OK == MV_OK) {OK = cam.start_grabbing();}

        if(OK == MV_OK) {OK = cam.get_exposure_time();}
        if(OK == MV_OK) {OK = cam.get_gain_value();}
        if(OK == MV_OK) {OK = cam.get_auto_exposure();}

        if(OK == MV_OK) {OK = cam.set_auto_exposure(2);}
        if(OK == MV_OK) {OK = cam.set_gain_value(4.0);}

        if(OK == MV_OK) 
        {
            int fail_count = 0;
            while(true)
            {
                OK = cam.get_image(img);
                if (OK == MV_OK)
                {
                    fail_count = 0;
                    cv::imshow("image", img);
                    int key = cv::waitKey(1);
                    if(key == ' ' || key == 27) {break;}
                }
                else
                {
                    fail_count++;
                    if (fail_count > 100) {break;}
                }
            }
        }
        
        OK = cam.stop_grabbing();
        if(OK == MV_OK) {OK = cam.close_device();}

    }
    
    Camera::cc_finalize();
    return OK;
}

