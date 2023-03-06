#include "CameraParam.h"

/**
 * @brief 获取Lib版本信息
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
const char* GX_CC_GetSDKVersion();

/**
 * @brief 打开Lib库
 */
void GX_CC_InitLib();


/**
 * @brief 关闭Lib库
 */
void GX_CC_ReleaseLib();

/**
 * @brief 获取设备数量
 *
 * @param[in] dev_num 返回设备列表的地址
 * @param[in] time_out 超时
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_GetDeviceList(uint32_t *dev_num, uint32_t time_out);

/**
 * @brief 枚举设备列表，获取设备基本信息
 *
 * @param[in] device_info_list 用户分配的地址空间
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_EnumDevices(GX_CC_DEVICE_INFO_LIST* device_info_list);

/**
 * @brief 打开设备
 *
 * @param[in] handle 返回设备句柄的地址
 * @param[in] pstDevInfo 指定设备的指针
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_OpenDevice(void **handle, const GX_CC_DEVICE_INFO *device_info_pointer);

/**
 * @brief 关闭设备
 *
 * @param[in] handle 设备句柄
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_CloseDevice(void *handle);

/**
 * @brief 打开采集
 *
 * @param[in] handle 设备句柄
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_StartGrabbing(void *handle);

/**
 * @brief 关闭采集
 *
 * @param[in] handle 设备句柄
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_StopGrabbing(void *handle);

/**
 * @brief 获取一帧图像数据, 用户获取到图像数据后应检查图像状态标志是否成功，参见
 * GX_FRAME_STATUS
 *
 * @param[in] handle 设备句柄
 * @param[in] frame_data_addr 用户分配的地址空间
 * @param[in] time_out 获取超时
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_GetOneFrameTimeout(void *handle, GX_FRAME_DATA *frame_data_addr, uint32_t time_out);

/**
 * @brief 根据设备MAC地址配置IP，MAC地址可通过枚举设备列表获取
 *
 * @param[in] MAC 设备的MAC地址
 * @param[in] IP 将配置的IP地址
 * @param[in] subnet_mask 将配置的子网掩码
 * @param[in] default_gateway 将配置的网关地址
 * @param[in] user_ID 将配置的用户ID
 * @param[in] mode 配置模式，参见GX_IP_CONFIGURE_MODE
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_IPConfiguration(const char *MAC,
                          const char *IP,
                          const char *subnet_mask,
                          const char *default_gateway,
                          const char *user_ID,
                          GX_IP_CONFIGURE_MODE mode);

/**
 * @brief 配置一个int64_t类型的值
 *
 * @param[in] handle 设备句柄
 * @param[in] feature_ID 功能码，参见GX_FEATRUE_ID
 * @param[in] value 要设置的值
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_SetIntValue(void *handle, GX_FEATURE_ID feature_ID, int64_t value);

/**
 * @brief 获取一个int64_t类型的值到指定地址
 *
 * @param[in] handle 设备句柄
 * @param[in] feature_ID 功能码，参见GX_FEATRUE_ID
 * @param[in] address 存值地址
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_GetIntValue(void *handle, GX_FEATURE_ID feature_ID, int64_t *address);

/**
 * @brief 注册采集回调函数
 *
 * @param[in] handle 设备句柄
 * @param[in] func_param 回调函数持有的参数地址
 * @param[in] func 回调函数，类型参见GXDeviceOfflineCallBack
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_RegisterCaptureCallback(void *handle, void *func_param, GXCaptureCallBack func);

/**
 * @brief 注销捕获回调函数
 *
 * @param[in] handle 设备句柄
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_UnregisterCaptureCallback(void *handle);

/**
 * @brief 用户可以通过此接口注册设备掉线处理回调函数
 *
 * @param[in] handle 设备句柄
 * @param[in] func_param_addr 用户事件处理函数持有的参数地址
 * @param[in] func 用户事件处理回调函数，类型参见GXDeviceOfflineCallBack
 * @param[in] func_handle 掉线回调函数句柄，此句柄用来注销回调函数
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_RegisterDeviceOfflineCallback(void *handle,
                                        void *func_param_addr,
                                        GXDeviceOfflineCallBack func,
                                        void *func_handle);

/**
 * @brief 用户可以通过此接口注销设备掉线处理回调函数
 *
 * @param[in] handle 设备句柄
 * @param[in] func_handle 掉线回调函数句柄
 *
 * @return 成功返回0，错误返回错误码，参见GX_STATUS_LIST
 */
int GX_CC_UnregisterDeviceOfflineCallback(void *handle, void *func_handle);

