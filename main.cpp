#include <Camera.h>

int Camera::nRet;

int main()
{
    Camera::init();

{
    Camera cam("nikon");

    cam.enum_camera();

    cam.create_handle();

    cam.open_camera();

    cam.start_grabbing();

    cam.stop_grabbing();
}

    Camera::finalize();

    return 0;
}
