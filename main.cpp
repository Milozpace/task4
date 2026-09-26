#include <Camera.h>


int main()
{
    int record = 1;
    {
        Camera cam1;

        Camera::search_camera();//未初始化，搜索失败测试
        record = Camera::set_up();
        
        if(record == 1)
        {
            record = cam1.run_camera();
        }
        
        if(record == 1)
        {
            record = cam1.capture_image();
        }

        if(record == 1)
        {
            record = cam1.close_camera();
        }

        if(record == 1) record = 0;
        
    }

    if(Camera::finalize() != 1) return -2;

    return record;
}
