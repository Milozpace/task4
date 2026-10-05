#include <Camera.h>
#include <SdkCtrl.h>


int main()
{
    int OK = MV_OK;
    MV_CC_DEVICE_INFO_LIST stDeviceList;

    OK = cc_initailize();

    if(OK == MV_OK) {OK = cc_enum_devices(stDeviceList);}
    
    if(OK == MV_OK)
    {
        Camera cam;
        cam.create_handle(stDeviceList); 
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
            while(true)
            {
                OK = cam.get_image(img);
                
                cv::imshow("image", img);
                int key = cv::waitKey(1);
                if(key == ' ' || key == 27) {break;}
            }
        }
        
        if(OK == MV_OK) {OK = cam.stop_grabbing();}
        if(OK == MV_OK) {OK = cam.close_device();}

    }
    
    cc_finalize();
    return OK;
}

