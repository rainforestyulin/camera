#include "DHCameraControl.h"

#include <stdio.h>
#include <stdlib.h>

#define CHECK(x) if(x != 0){printf("Error code:(%d) in (%s) line(%d)\n",x,__FILE__,__LINE__); }

void GX_CC_InitLib();
const char* GX_CC_GetSDKVersion();
int main()
{
    int res = 0;
    GX_CC_DEVICE_INFO_LIST deviceList;
    GX_FRAME_DATA frameData;
    frameData.pImgBuf = malloc(2448*2048);
    deviceList.pDeviceInfo[0] = (GX_CC_DEVICE_INFO*)malloc(sizeof(GX_CC_DEVICE_INFO));
    GX_CC_InitLib();
    res = GX_CC_EnumDevices(&deviceList);
    CHECK(res);
    printf("%d\n",deviceList.pDeviceInfo[0]->deviceIndex);
    printf("%s\n",deviceList.pDeviceInfo[0]->stGigEInfo.szMAC);
    res = GX_CC_IPConfiguration("00-21-49-03-90-EE","172.20.30.49","255.255.255.0","172.20.30.1","DaHeng",GX_IP_CONFIGURE_STATIC_IP);
    CHECK(res);
   printf("%s\n",GX_CC_GetSDKVersion());
    void *handle;
    res = GX_CC_OpenDevice(&handle,deviceList.pDeviceInfo[0]);
    CHECK(res);
    res = GX_CC_StartGrabbing(handle);
    CHECK(res);
    res = GX_CC_GetOneFrameTimeout(handle,&frameData,10000);
    CHECK(res);
    int64_t val;
    res = GX_CC_GetIntValue(handle,GX_INT_PAYLOAD_SIZE,&val);
    CHECK(res);
    printf("%ld",val);
    GX_CC_StopGrabbing(handle);
    GX_CC_CloseDevice(handle);
    free(deviceList.pDeviceInfo[0]);
    free(frameData.pImgBuf);
    return 0;
}
