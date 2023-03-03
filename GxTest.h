#include "GxIAPI.h"
#include "DxImageProc.h"
#include <ostream>
#include <vector>
#include <map>
/* 摄像头使用的流程
 * 用户调用GetDeviceList获取摄像头列表:句柄类型为int32
 * 调用OpenDevice打开摄像头，句柄类型为{int, hDevice};
 * 在之后的操作中配置参数包括采取图像均通过句柄可以唯一确定
 * */

union Param
{
    // SN
    char szSN[GX_INFO_LENGTH_32_BYTE];
    // IP
    char szIP[GX_INFO_LENGTH_32_BYTE];
    // MAC
    char szMAC[GX_INFO_LENGTH_32_BYTE];
    // Index
    uint32_t szIndex;
    // UserID
    char szUserID[GX_INFO_LENGTH_64_BYTE + 4];
};

class OpenParam
{
public:
    OpenParam() = delete;
    // paramType param handle
    OpenParam(GX_OPEN_MODE_CMD paramType, uint32_t szIndex, GX_DEV_HANDLE* pHDevice)
        :paramType_(paramType), pHDevice_(pHDevice){
            param_.szIndex = szIndex;
        }
    OpenParam(GX_OPEN_MODE_CMD paramType,const char* szChar, GX_DEV_HANDLE *pHDevice)
        :paramType_(paramType), pHDevice_(pHDevice){
            if (paramType_ == GX_OPEN_USERID)
            {
                for (size_t i = 0; i < GX_INFO_LENGTH_64_BYTE+4; ++i)
                {
                    param_.szUserID[i] = *(szChar + i);
                }
            }
            else
            {
                for (size_t i = 0; i < GX_INFO_LENGTH_32_BYTE; ++i)
                {
                    param_.szUserID[i] = *(szChar + i);
                }
            }
        }
    const GX_DEV_HANDLE& GetHandle() const
    {
        return *pHDevice_;
    }
    Param param_;
    // reference to GX_OPEN_MODE
private:
    GX_OPEN_MODE_CMD paramType_;

    GX_DEV_HANDLE* pHDevice_;
};


class CameraSDK
{
public:

    //
    CameraSDK();

    //
    CameraSDK(const CameraSDK& T) = delete;

    //
    ~CameraSDK();

    // 获取Lib版本号
    const char* GetLibVersion();

    // 获取摄像头列表, 默认超时1000ms
    GX_STATUS GetDeviceList();

    // 获取所有设备的基础信息
    GX_STATUS GetDeviceBaseInfo();

    // 获取所有设备的网络信息
    GX_STATUS GetDeviceIPInfo();

    // 配置相机IP
    // GX_IP_CONFIGURE_MODE:
    //      GX_IP_CONFIGURE_STATIC_IP
    //      GX_IP_CONFIGURE_DHCP
    //      GX_IP_CONFIGURE_LLA
    //      GX_IP_CONFIGURE_DEFAULT
    GX_STATUS IPConfiguration(char *MAC,
                         char* IP,
                         char* subnetMask,
                         char* defaultGateway,
                         char* userID,
                         GX_IP_CONFIGURE_MODE mode = GX_IP_CONFIGURE_DHCP);

    // 注册回调函数
    GX_STATUS RegisterCallBack(GX_DEV_HANDLE &hDevice, void *pUserParam, GXCaptureCallBack callBackFun);

    // 注销回调函数
    GX_STATUS UnregisterCallBack(GX_DEV_HANDLE &hDevice);

    // 使用序号打开设备
    GX_STATUS OpenDevice(uint32_t nDeviceIndex);


    // 关闭摄像头
    GX_STATUS CloseDevice(GX_DEV_HANDLE &hDevice);

    // 开始取流
    GX_STATUS StartGrabbing(GX_DEV_HANDLE &hDevice);

    // 停止取流
    GX_STATUS StopGrabbing(GX_DEV_HANDLE &hDevice);

    // 设置一个矩形感兴趣区域
    GX_STATUS SetROI(GX_DEV_HANDLE &hDevice,
                     int64_t offSetX,
                     int64_t offSetY,
                     int64_t width, int64_t height);

    // 设置相机曝光参数
    GX_STATUS SetExposureTime(GX_DEV_HANDLE &hDevice, float usMin, float usMax);

    // 获取一帧数据
    // pFrameData->pImgBuf 需提前分配好内存并且在使用完毕后释放
    // 默认超时1000ms
    GX_STATUS GetOneFrameTimeOut(GX_DEV_HANDLE &hDevice, GX_FRAME_DATA *pFrameData, uint32_t timeOut = 1000);

    // 设置Enum类型
    int32_t SetEnumValue(std::string name);

    // 设置滚动条纹测试图 0-关闭 1-打开
    GX_STATUS SetTestPattern(GX_DEV_HANDLE &hDevice, uint32_t IO);

    // 设置采集帧率 过高会导致残帧
    GX_STATUS SetGrabbingFPS(GX_DEV_HANDLE &hDevice, float fps);

    // 使能外部触发源
    // 默认为line0
    GX_STATUS EnableTriggerOnLine(GX_DEV_HANDLE &hDevice, uint32_t lineIndex = 0);

    // 关闭外部触发源
    GX_STATUS DisableTriggerOnLine(GX_DEV_HANDLE &hDevice);

    // 设置设备心跳超时
    GX_STATUS SetHeartBeatTimeout(GX_DEV_HANDLE &hDevice, int64_t &nValue);

    const uint32_t GetDeviceNum();

    const GX_DEVICE_BASE_INFO *GetDeviceInfo();

    const std::vector<OpenParam> &GetOpenedList();

    const GX_DEVICE_IP_INFO *GetAllDeviceIPInfo();

    const GX_DEV_HANDLE GetHandle(uint32_t index);

    // 获取当前配置下的预估带宽
    GX_STATUS GetEstimatedBandwidth(GX_DEV_HANDLE &hDevice, int64_t *bandwidth);

    // 打印当前设备支持的像素格式
    void PrintPixelFormat(GX_DEV_HANDLE &hDevice);

    // 打印摄像头的基础参数
    void PrintBaseInfo();

    // 打印摄像头的IP 参数
    void PrintIPInfo();
private:

    ///-------------------------------<
    //此型号不支持的接口列表

    // 获取一帧RGB数据
    bool GetImageForRGB(GX_DEV_HANDLE &hDevice, void *pImgBuf, uint32_t timeOut = 1000);

    // 获取一帧BGR数据
    bool GetImageForBGR(GX_DEV_HANDLE &hDevice, void *pImgBuf, uint32_t timeOut = 1000);

    // 获取一帧GRB数据
    bool GetImageForGRB(GX_DEV_HANDLE &hDevice, void *pImgBuf, uint32_t timeOut = 1000);

    // 获取一帧GBR数据
    bool GetImageForGBR(GX_DEV_HANDLE &hDevice, void *pImgBuf, uint32_t timeOut = 1000);

    // 以合并的方式降低分辨率
    // 当前相机不支持
    GX_STATUS SetBinning(GX_DEV_HANDLE &hDevice,
                         int64_t nBinningH,
                         int64_t nBinningV,
                         int64_t nDecimationH,
                         int64_t nDecimationV);

    // 获取XXX像素格式的帧数据
    bool GetImageForXXX(GX_DEV_HANDLE &hDevice, int64_t nPixelFormat, void *pImgBuf, uint32_t timeOut = 1000 /* ms */);

    // open device by MAC/IP/SN/UserID
    bool OpenDevice(GX_OPEN_MODE_CMD paramType, char * param);

    // open device by SN
    bool OpenDeviceBySN(char *SN);

    // open device by MAC
    bool OpenDeviceByMAC(char *MAC);

    // open device by UserID
    bool OpenDeviceByUserID(char *UserID);

private:

    // Log file_name
    std::ofstream *log_;

    // Error msg
    std::ostream *errorOs_;

    // Error
    std::ostream& PrintError(std::ostream &os, GX_STATUS status);

    // Log
    void Log(std::ofstream &fp, const char *log);
    void Log(std::ostream &os, const char *log);

    void PrintFrameStatus(GX_FRAME_STATUS fStatus);


    ///-----------------------------
    // 当前在线的摄头数量
    uint32_t nDeviceNum_;

    // 所有设备的基础信息
    GX_DEVICE_BASE_INFO *pBaseinfo_;

    // 所有设备的网络信息
    GX_DEVICE_IP_INFO *pIPinfo_;

    // 记录打开状态的摄像头即句柄
    // std::map<OpenParam/* OpenParam */, GX_DEV_HANDLE/* hDevice */>  openedDevice;
    std::map<char*, GX_DEV_HANDLE> SN_Handle_;
    std::map<char*, GX_DEV_HANDLE> MAC_Handle_;
    std::map<char*, GX_DEV_HANDLE> IP_Handle_;
    std::map<char*, GX_DEV_HANDLE> Name_Handle_;
    std::map<uint32_t, GX_DEV_HANDLE> Index_Handle_;
    std::vector<OpenParam> openedDevice_;

};
