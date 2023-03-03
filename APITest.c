#include "DHCameraControl.h"
#include "CameraParam.h"

#include <stdio.h>
#include <stdlib.h>

void GX_CC_InitLib();
const char* GX_CC_GetSDKVersion();
int main()
{
    GX_CC_DEVICE_INFO_LIST deviceList;
    GX_CC_InitLib();
    GX_CC_EnumDevices(&deviceList);
    printf(GX_CC_GetSDKVersion());
    return 0;
}
