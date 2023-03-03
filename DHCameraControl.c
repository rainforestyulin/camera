#include "CameraParam.h"
#include "GxIAPI.h"
#include <stdlib.h>
#include <stdbool.h>

#define true 1
#define false 0
#define CHECK_RESULT(X) if(X != GX_STATUS_SUCCESS) return X

/**
 * @brief Get SDK Version
 *
 * @return const char* type pointer to SDK version
 */
const char* GX_CC_GetSDKVersion()
{
    return GXGetLibVersion();
}

/**
 * @brief Init Device Lib
 */
void GX_CC_InitLib()
{
    GXInitLib();
}

/**
 * @brief Close Device Lib
 */
void GX_CC_ReleaseLib()
{
    GXCloseLib();
}

/**
 * @brief Get Device List
 *
 * @param[in] pstDevList GX_CC_DEVICE_INFO_LIST type poniter to malloced struct
 *
 * @return Success, return GX_STATUS_SUCCESS.Failure, return error code
 */
int GX_CC_EnumDevices(GX_CC_DEVICE_INFO_LIST* pstDevList)
{
    GX_STATUS status = GXUpdateDeviceList(&(pstDevList->nDeviceNum), 1000);
    CHECK_RESULT(status);
    for (size_t i = 0; i < pstDevList->nDeviceNum; ++i)
    {
        pstDevList->pDeviceInfo[i]->deviceIndex = i + 1;
        size_t base_info_size = sizeof(GX_DEVICE_BASE_INFO);
        status = GXGetAllDeviceBaseInfo(&(pstDevList->pDeviceInfo[i]->stBaseInfo), &base_info_size);
        CHECK_RESULT(status);
        status = GXGetDeviceIPInfo(i, &(pstDevList->pDeviceInfo[i]->stGigEInfo));
        CHECK_RESULT(status);
    }
    return status;
}

/**
 * @brief Open Device
 *
 * @param[in] handle Device Handle
 * @param[in] pstDevInfo Open Param
 *
 * @return Success, return GX_STATUS_SUCCESS.Failure return error code
 */
int GX_CC_OpenDevice(void **handle, const GX_CC_DEVICE_INFO *pstDevInfo)
{
    GX_STATUS status = GXOpenDeviceByIndex(pstDevInfo->deviceIndex, handle);
    CHECK_RESULT(status);
    return status;
}

/**
 * @brief Close Device
 *
 * @param[in] handle Handle of to close device
 *
 * @return Success, return GX_STATUS_SUCCESS, Failure return error code
 */
int GX_CC_CloseDevice(void *handle)
{
    GX_STATUS status = GXCloseDevice(handle);
    return status;
}

/**
 * @brief Start Grabbing
 *
 * @param[in] handle Device Handle
 *
 * @return Success, return GX_STATUS_SUCCESS.Failure, return error code
 */
int GX_CC_StartGrabbing(void *handle)
{
    GX_STATUS status = GXStreamOn(handle);
    return status;
}

/**
 * @brief Stop Grabbing
 *
 * @param[in] handle Device Handle
 *
 * @return Success, return GX_STATUS_SUCCESS.Failure, return error code
 */
int GX_CC_StopGrabbing(void *handle)
{
    GX_STATUS status = GXStreamOff(handle);
    return status;
}

/**
 * @brief Get One Frame Data In Given Time
 *
 * @param[in] handle Device Handle
 * @param[in] pFrameData A Structure Type Of GX_FRAME_DATA
 * @param[in] timeOut Given Time
 *
 * @return Success, return GX_STATUS_SUCCESS.Failure return error code.
 *                  Including Frame Status.
 */
int GX_CC_GetOneFrameTimeout(void *handle, GX_FRAME_DATA *pFrameData, uint32_t timeOut)
{
    GX_STATUS status = GXGetImage(handle,pFrameData, timeOut);
    CHECK_RESULT(status);
    if(pFrameData->nStatus != GX_FRAME_STATUS_SUCCESS)
    {
    	return pFrameData->nStatus;
    }
    return status;
}

/**
 * @brief Set IP Parameters According To MAC
 *
 * @param[in] MAC Device MAC  Address
 * @param[in] IP Const Char* Type Address Which Indicate Device IP Address
 * @param[in] subnetMask Subnetmask
 * @param[in] defaultGateWay DefaultGateWay
 * @param[in] userID UserID Of Net
 * @param[in] mode IP Configure Mode
 *
 * @return Success, return GX_STATUS_SUCCESS.Failure, return error code
 */
int GX_CC_IPConfiguration(const char *MAC,
                          const char *IP,
                          const char *subnetMask,
                          const char *defaultGateWay,
                          const char *userID,
                          GX_IP_CONFIGURE_MODE mode)
{
    GX_STATUS status = GXGigEIpConfiguration(MAC, mode, IP, subnetMask, defaultGateWay, userID);
    return status;
}
