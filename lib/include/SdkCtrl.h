#pragma once
#include <iostream>

#include <MvCameraControl.h>

int cc_initailize();

int cc_enum_devices(MV_CC_DEVICE_INFO_LIST& list);

int cc_finalize();

