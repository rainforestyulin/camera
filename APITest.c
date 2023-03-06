#include "DHCameraControl.h"

#include <stdio.h>
#include <stdlib.h>

#define CHECK(x) if(x != 0){printf("Error code:(%d) in (%s) line(%d)\n",x,__FILE__,__LINE__); }

static void OnDeivceOffline(void* pUserParam)
{
    printf("device offline\n");
    return;
}

void GX_CC_InitLib();
const char* GX_CC_GetSDKVersion();
int main()
{
GX_CC_InitLib();
    uint32_t device_num = 0;
    GX_CC_GetDeviceList(&device_num, 1000);
    int res = 0;
    GX_CC_DEVICE_INFO_LIST device_list;
    GX_FRAME_DATA frameData;
    frameData.pImgBuf = malloc(2448*2048);
    device_list.device_info[0] = (GX_CC_DEVICE_INFO*)malloc(sizeof(GX_CC_DEVICE_INFO)*device_num);
    res = GX_CC_EnumDevices(&device_list);
    CHECK(res);
    printf("%d\n",device_list.device_info[0]->device_index);
    printf("%s\n",device_list.device_info[0]->device_ip_info.szMAC);
    res = GX_CC_IPConfiguration("00-21-49-03-90-EE","172.20.30.49","255.255.255.0","172.20.30.1","DaHeng",GX_IP_CONFIGURE_STATIC_IP);
    CHECK(res);
    printf("%s\n",GX_CC_GetSDKVersion());
    /*void *handle;
    res = GX_CC_OpenDevice(&handle,device_list.device_info[0]);
    CHECK(res);
    GX_EVENT_CALLBACK_HANDLE chandle;
    res = GX_CC_RegisterDeviceOfflineCallback(handle, NULL, OnDeivceOffline, handle);
    CHECK(res);
    res = GX_CC_StartGrabbing(handle);
    CHECK(res);
    res = GX_CC_GetOneFrameTimeout(handle,&frameData,10000);
    CHECK(res);
    int64_t val;
    res = GX_CC_GetIntValue(handle,GX_INT_PAYLOAD_SIZE,&val);
    CHECK(res);

    GX_CC_StopGrabbing(handle);
    GX_CC_CloseDevice(handle);
    */
    free(device_list.device_info[0]);
    free(frameData.pImgBuf);
    return 0;
}
