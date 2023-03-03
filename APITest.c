#include "DHCameraControl.h"

#include <stdio.h>
#include <stdlib.h>

void GX_CC_InitLib();
const char* GX_CC_GetSDKVersion();
int main()
{
    GX_CC_DEVICE_INFO_LIST deviceList;
    GX_FRAME_DATA frameData;
    frameData.pImgBuf = malloc(2448*2048);
    deviceList.pDeviceInfo[0] = (GX_CC_DEVICE_INFO*)malloc(sizeof(GX_CC_DEVICE_INFO));
    GX_CC_InitLib();
    GX_CC_EnumDevices(&deviceList);
    printf("%d\n",deviceList.nDeviceNum);
    printf("%d\n",deviceList.pDeviceInfo[0]->deviceIndex);
    printf("%s\n",deviceList.pDeviceInfo[0]->stBaseInfo.szModelName);
    printf("%s\n",GX_CC_GetSDKVersion());
    void *handle;
    GX_CC_OpenDevice(&handle,deviceList.pDeviceInfo[0]);
    GX_CC_StartGrabbing(handle);
    int res = GX_CC_GetOneFrameTimeout(handle,&frameData,10000);
    printf("%d\n",res);
    GX_CC_StopGrabbing(handle);
    GX_CC_CloseDevice(handle);
    free(deviceList.pDeviceInfo[0]);
    free(frameData.pImgBuf);
    return 0;
}
