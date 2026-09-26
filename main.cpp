#include <Camera.h>


int main()
{
    int record = 1;
    {
        Camera cam1;

        record = Camera::set_up();
        
        if(record == 1) {record = cam1.run_camera();}

        if(record == 1) {record = cam1.get_image();}

        if(record == 1) {record = cam1.stop_run_camera();}
            
    
        if(record == 1) record = 0;
    }

    if(Camera::finalize() != 1) return -2;

    return record;
}
