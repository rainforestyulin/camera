#include "CameraParam.h"


const char* GX_CC_GetSDKVersion();
void GX_CC_InitLib();
void GX_CC_ReleaseLib();
int GX_CC_EnumDevices(GX_CC_DEVICE_INFO_LIST* pstDevList);
int GX_CC_OpenDevice(void **handle, const GX_CC_DEVICE_INFO *pstDevInfo);
int GX_CC_StartGrabbing(void *handle);
int GX_CC_StopGrabbing(void *handle);
int GX_CC_GetOneFrameTimeout(void *handle, GX_FRAME_DATA *pFrameData, uint32_t timeOut);
int GX_CC_IPConfiguration(const char *MAC,
                          const char *IP,
                          const char *subnetMask,
                          const char *defaultGateWay,
                          const char *userID,
                          GX_IP_CONFIGURE_MODE mode);

