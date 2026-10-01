#include <SdkCtrl.h>

int cc_initailize()
{
    int nRet = MV_CC_Initialize();
    return nRet;
}

int cc_enum_devices(MV_CC_DEVICE_INFO_LIST list)
{
    int nRet = MV_CC_EnumDevices(MV_USB_DEVICE, &list);
    return nRet;
}

int cc_finalize()
{
    int nRet = MV_CC_Finalize();
    return nRet;
}