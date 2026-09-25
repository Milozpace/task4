#include <Camera.h>

int Camera::nRet;


int main()
{
    if(Camera::init() != 1) return -1;

    Camera::run_camera();

    Camera::finalize();

    return 0;
}
