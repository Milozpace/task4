#include <Camera.h>


int main()
{
    bool ok = true;
    {
        Camera cam1;

        ok = Camera::set_up();
        
        if(ok == 1) {ok = cam1.run_camera();}

        if(ok == 1) {ok = cam1.get_image();}

        if(ok == 1) {ok = cam1.stop_run_camera();}
            
    
        if(ok == 1) {ok = false;}
    }

    if(Camera::finalize() != true) return -2;

    return ok;
}
