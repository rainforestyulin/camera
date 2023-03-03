#include "GxTest.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"


#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <ctime>
#include <string.h>

// #define DEBUG
#ifdef DEBUG
#define ErrP(X) std::cout<<"Error code:"<<(X)<<"  In>"<<__FILE__<<"  Line:"<<__LINE__<<std::endl
#endif

CameraSDK::CameraSDK():
    pBaseinfo_(nullptr),
    pIPinfo_(nullptr)
{

    // 初始化Lib库
    GXInitLib();

    // 获取摄像头列表
    GetDeviceList();

}
CameraSDK::~CameraSDK()
{
    GXCloseLib();
    if (pBaseinfo_)
    {
       delete [] pBaseinfo_;
    }
    if (pIPinfo_)
    {
        delete [] pIPinfo_;
    }
}
const char* CameraSDK::GetLibVersion()
{
    return GXGetLibVersion();
}

GX_STATUS CameraSDK::GetDeviceList()
{
    GX_STATUS status = GXUpdateDeviceList(&nDeviceNum_, 1000);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
#ifdef DEBUG
        ErrP(status);
#endif
    }
    return status;
}

GX_STATUS CameraSDK::GetDeviceBaseInfo()
{
    GetDeviceList();
    if (pBaseinfo_ ==  nullptr)
    {
        pBaseinfo_ = new GX_DEVICE_BASE_INFO[nDeviceNum_];
    }
    else
    {
        delete [] pBaseinfo_;
        pBaseinfo_ = new GX_DEVICE_BASE_INFO[nDeviceNum_];
    }
    size_t nSize = nDeviceNum_ * sizeof(GX_DEVICE_BASE_INFO);
    GX_STATUS status = GX_STATUS_SUCCESS;
    status = GXGetAllDeviceBaseInfo(pBaseinfo_, &nSize);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
#ifdef DEBUG
        ErrP(status);
#endif
    }
    return status;
}

GX_STATUS CameraSDK::GetDeviceIPInfo()
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    if (pIPinfo_ == nullptr)
    {
        pIPinfo_ = new GX_DEVICE_IP_INFO[nDeviceNum_];
    }
    else
    {
        delete [] pIPinfo_;
        pIPinfo_ = new GX_DEVICE_IP_INFO[nDeviceNum_];
    }
    for (size_t i = 0; i < nDeviceNum_ && status == GX_STATUS_SUCCESS; ++i)
    {
        status = GXGetDeviceIPInfo(i+1, &pIPinfo_[i]);
    }
    return status;
}

GX_STATUS CameraSDK::IPConfiguration(char* MAC,
                                char* IP,
                                char* subnetMask,
                                char* defaultGateway,
                                char* userID,
                                GX_IP_CONFIGURE_MODE mode)
{
    GX_STATUS status = GXGigEIpConfiguration(MAC, mode, IP, subnetMask, defaultGateway, userID);
    return status;
}

GX_STATUS CameraSDK::RegisterCallBack(GX_DEV_HANDLE &hDevice, void *pUserParam, GXCaptureCallBack callBackFun)
{
    GX_STATUS status = GXRegisterCaptureCallback(hDevice, pUserParam, callBackFun);
    return status;
}

GX_STATUS CameraSDK::UnregisterCallBack(GX_DEV_HANDLE &hDevice)
{
    GX_STATUS status = GXUnregisterCaptureCallback(hDevice);
    return status;
}

GX_STATUS CameraSDK::OpenDevice(uint32_t nDeviceIndex)
{
    GX_DEV_HANDLE hDevice;
    GX_STATUS status = GXOpenDeviceByIndex(nDeviceIndex, &hDevice);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout,status);
#ifdef DEBUG
        ErrP(status);
#endif
    }
    Index_Handle_.insert(std::pair<uint32_t, GX_DEV_HANDLE>(nDeviceIndex, hDevice));
    return status;
}

// bool CameraSDK::OpenDevice(GX_OPEN_MODE_CMD paramType, char *param)
// {
//     GX_STATUS status = GX_STATUS_SUCCESS;
//     GX_DEV_HANDLE hDevice;
//     GX_OPEN_PARAM stOpenParam;
//     stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
//     switch (paramType)
//     {
//     case GX_OPEN_IP:
//         stOpenParam.openMode = GX_OPEN_IP;
//     break;
//     case GX_OPEN_SN:
//         stOpenParam.openMode = GX_OPEN_SN;
//         break;
//     case GX_OPEN_MAC:
//         stOpenParam.openMode = GX_OPEN_MAC;
//         break;
//     case GX_OPEN_USERID:
//         stOpenParam.openMode = GX_OPEN_USERID;
//         break;
//     default:
// #ifdef DEBUG
//         ErrP("Error: Unknown open mode");
// #endif
//         break;
//     }
//     stOpenParam.pszContent = param;
//     status = GXOpenDevice(&stOpenParam, &hDevice);
//     if (status != GX_STATUS_SUCCESS)
//     {
//         PrintError(std::cout,status);
// #ifdef DEBUG
//         ErrP(status);
// #endif
//         return false;
//     }
//     openedDevice_.emplace_back(OpenParam{paramType, param, &hDevice});
//     // openedDevice.insert(std::pair<OpenParam, GX_DEV_HANDLE>({GX_OPEN_IP, IP}, hDevice));
//     return true;
// }
//
bool CameraSDK::OpenDeviceBySN(char *SN)
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    GX_DEV_HANDLE hDevice;
    GX_OPEN_PARAM stOpenParam;
    stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
    stOpenParam.openMode = GX_OPEN_SN;
    stOpenParam.pszContent = SN;
    status = GXOpenDevice(&stOpenParam, &hDevice);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return false;
    }
    // openedDevice_.emplace_back(Args &&args...)
    return true;
}
GX_STATUS CameraSDK::CloseDevice(GX_DEV_HANDLE &hDevice)
{
    GX_STATUS status = GXCloseDevice(hDevice);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
    }
    return status;
}

GX_STATUS CameraSDK::StartGrabbing(GX_DEV_HANDLE &hDevice)
{
    GX_STATUS status = GXStreamOn(hDevice);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
    }
    return status;
}

GX_STATUS CameraSDK::StopGrabbing(GX_DEV_HANDLE &hDevice)
{
    GX_STATUS status = GXStreamOff(hDevice);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
    }
    return status;
}

GX_STATUS CameraSDK::SetROI(GX_DEV_HANDLE &hDevice, int64_t offSetX, int64_t offSetY, int64_t width, int64_t height)
{
    GX_STATUS status = GXSetInt(hDevice, GX_INT_WIDTH , width);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return status;
    }
    status = GXSetInt(hDevice, GX_INT_HEIGHT, height);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return status;
    }
    status = GXSetInt(hDevice, GX_INT_OFFSET_X, offSetX);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return status;
    }
    status = GXSetInt(hDevice, GX_INT_OFFSET_Y, offSetY);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return status;
    }
    return status;
}

GX_STATUS CameraSDK::SetBinning(GX_DEV_HANDLE &hDevice,
                                int64_t nBinningH,
                                int64_t nBinningV,
                                int64_t nDecimationH,
                                int64_t nDecimationV)
{
    GX_STATUS status = GXSetEnum(hDevice
                                 , GX_ENUM_BINNING_HORIZONTAL_MODE, GX_BINNING_HORIZONTAL_MODE_SUM);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return status;
    }
    status = GXSetEnum(hDevice, GX_ENUM_BINNING_VERTICAL_MODE, GX_BINNING_HORIZONTAL_MODE_SUM);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return status;
    }
    status = GXSetInt(hDevice, GX_INT_BINNING_HORIZONTAL, nDecimationH);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return status;
    }
    status = GXSetInt(hDevice, GX_INT_BINNING_VERTICAL, nDecimationV);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
    }
    return status;
}

GX_STATUS CameraSDK::SetExposureTime(GX_DEV_HANDLE &hDevice,
                                    float usMin,
                                    float usMax)
{
    GX_FLOAT_RANGE shutterRange;
    GX_STATUS status = GXGetFloatRange(hDevice, GX_FLOAT_EXPOSURE_TIME, &shutterRange);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return status;
    }
    if (usMin < shutterRange.dMin || usMax > shutterRange.dMax)
    {
        status = GX_STATUS_INVALID_PARAMETER;
        PrintError(std::cout, status);
        return  status;
    }
    status = GXSetEnum(hDevice, GX_ENUM_EXPOSURE_MODE, GX_EXPOSURE_AUTO_CONTINUOUS);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return status;
    }
    status = GXSetFloat(hDevice, GX_FLOAT_AUTO_EXPOSURE_TIME_MIN, usMin);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return status;
    }
    status = GXSetFloat(hDevice, GX_FLOAT_AUTO_EXPOSURE_TIME_MAX, usMax);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return status;
    }
    // status = GXSetEnum(hDevice, GX_ENUM_EXPOSURE_TIME_MODE, GX_EXPOSURE_TIME_MODE_ULTRASHORT);
    // if (status != GX_STATUS_SUCCESS)
    // {
    //     PrintError(std::cout, status);
    //     return status;
    // }
    return status;
}

GX_STATUS CameraSDK::GetOneFrameTimeOut(GX_DEV_HANDLE &hDevice,GX_FRAME_DATA *pFrameData, uint32_t timeOut)
{
    GX_STATUS status = GXGetImage(hDevice,pFrameData, timeOut);
    if (status == GX_STATUS_SUCCESS)
    {
        if(pFrameData->nStatus == GX_FRAME_STATUS_SUCCESS)
        {
            // 图像处理
        }
        else
        {
            PrintFrameStatus(pFrameData->nStatus);
        }
    }
    else
    {
        PrintError(std::cout, status);
    }
    return status;
}

bool CameraSDK::GetImageForXXX(GX_DEV_HANDLE &hDevice, int64_t nPixelFormat, void *pImgBuf, uint32_t timeOut)
{
    GX_STATUS status = GXSetEnum(hDevice, GX_ENUM_PIXEL_FORMAT, nPixelFormat);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return false;
    }
    GX_FRAME_DATA frameData;
    status = GXGetImage(hDevice, &frameData, timeOut);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
        return false;
    }
    memcpy(pImgBuf, frameData.pImgBuf, frameData.nImgSize);
    return true;
}

int32_t CameraSDK::SetEnumValue(std::string name)
{
    return 0;
}

GX_STATUS CameraSDK::SetTestPattern(GX_DEV_HANDLE &hDevice, uint32_t IO)
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    if (IO == 1)
    {
        status = GXSetEnum(hDevice, GX_ENUM_TEST_PATTERN, GX_ENUM_TEST_PATTERN_VERTICAL_LINE_MOVING);
    }
    else if (IO == 0)
    {
        status = GXSetEnum(hDevice, GX_ENUM_TEST_PATTERN, GX_ENUM_TEST_PATTERN_OFF);
    }
    else
    {
        PrintError(std::cout, GX_STATUS_INVALID_PARAMETER);
    }
    return status;
}

GX_STATUS CameraSDK::SetGrabbingFPS(GX_DEV_HANDLE &hDevice, float fps)
{
    GX_STATUS status = GXSetEnum(hDevice, GX_ENUM_ACQUISITION_FRAME_RATE_MODE, GX_ACQUISITION_FRAME_RATE_MODE_ON);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
    }
    status = GXSetFloat(hDevice, GX_FLOAT_ACQUISITION_FRAME_RATE, fps);
    return status;
}

GX_STATUS CameraSDK::EnableTriggerOnLine(GX_DEV_HANDLE &hDevice, uint32_t lineIndex)
{
    GX_STATUS status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
    if (status != GX_STATUS_SUCCESS)
    {
        return status;
    }
    switch (lineIndex)
    {
    case 0:
        status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_SOURCE, GX_TRIGGER_SOURCE_LINE0);
        break;
    case 1:
        status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_SOURCE, GX_TRIGGER_SOURCE_LINE1);
        break;
    case 2:
        status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_SOURCE, GX_TRIGGER_SOURCE_LINE2);
        break;
    case 3:
        status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_SOURCE, GX_TRIGGER_SOURCE_LINE3);
        break;
    default:
        status = GX_STATUS_INVALID_PARAMETER;
        break;
    }
    return status;
}

GX_STATUS CameraSDK::DisableTriggerOnLine(GX_DEV_HANDLE &hDevice)
{
    GX_STATUS status = GXSetEnum(hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
    }
    return status;
}

GX_STATUS CameraSDK::SetHeartBeatTimeout(GX_DEV_HANDLE &hDevice, int64_t &nValue)
{
    GX_STATUS status = GXSetInt(hDevice, GX_INT_GEV_HEARTBEAT_TIMEOUT, nValue);
    if (status != GX_STATUS_SUCCESS)
    {
        PrintError(std::cout, status);
    }
    return status;
}

const uint32_t CameraSDK::GetDeviceNum()
{
    return nDeviceNum_;
}
const GX_DEVICE_BASE_INFO *CameraSDK::GetDeviceInfo()
{
    return pBaseinfo_;
}
const GX_DEVICE_IP_INFO *CameraSDK::GetAllDeviceIPInfo()
{
    return pIPinfo_;
}

const std::vector<OpenParam> &CameraSDK::GetOpenedList()
{
    return openedDevice_;
}
std::ostream& CameraSDK::PrintError(std::ostream &os, GX_STATUS status)
{
    switch (status)
    {
    case GX_STATUS_NOT_INIT_API:
        os<<"Error: NOT_INIT_API"<<std::endl;
        break;
    case GX_STATUS_INVALID_HANDLE:
        os<<"Error: INVALID_HANDLE"<<std::endl;
        break;
    case GX_STATUS_INVALID_PARAMETER:
        os<<"Error: INVALID_PARAMETER"<<std::endl;
        break;
    case GX_STATUS_INVALID_CALL:
        os<<"Error: INVALID_CALL"<<std::endl;
        break;
    case GX_STATUS_TIMEOUT:
        os<<"Error: TIMEOUT"<<std::endl;
        break;
    case GX_STATUS_NOT_FOUND_TL:
        os<<"Error: The TL library cannot be found"<<std::endl;
        break;
    case GX_STATUS_OUT_OF_RANGE:
        os<<"Error: OUT_OF_RANGE"<<std::endl;
        break;
    case GX_STATUS_NOT_FOUND_DEVICE:
        os<<"Error: DEVICE_NOT_FOUND"<<std::endl;
    case GX_STATUS_NOT_IMPLEMENTED:
        os<<"Error: NOT_SUPPORTED"<<std::endl;
        break;
    default:
        os<<"Unknown Error code:"<<status<<std::endl;
        break;
    }
    return os;
}
void CameraSDK::PrintFrameStatus(GX_FRAME_STATUS fStatus)
{
    switch (fStatus)
    {
    case GX_FRAME_STATUS_SUCCESS:
        std::cout<<"OK: Normal Frame"<<std::endl;
        break;
    case GX_FRAME_STATUS_INCOMPLETE:
        std::cout<<"Frame Error: INCOMPLETE_FRAME"<<std::endl;
        break;
    case GX_FRAME_STATUS_INVALID_IMAGE_INFO:
        std::cout<<"Frame Error: INVALID_IMAGE_INFO"<<std::endl;
        break;
    default:
        std::cout<<"Unknown Frame Error code:"<<fStatus<<std::endl;
        break;
    }
}

void Log(std::ostream &os, const char *log)
{
    os<<log<<std::endl;
}

const GX_DEV_HANDLE CameraSDK::GetHandle(uint32_t index)
{
    GX_DEV_HANDLE handle = nullptr;
    if (Index_Handle_.find(index) != Index_Handle_.end())
    {
        handle = Index_Handle_.find(index)->second;
    }
    return handle;
}
GX_STATUS CameraSDK::GetEstimatedBandwidth(GX_DEV_HANDLE &hDevice, int64_t *bandwidth)
{
    GX_STATUS status = GXGetInt(hDevice, GX_INT_ESTIMATED_BANDWIDTH, bandwidth);
    if (status != GX_STATUS_SUCCESS )
    {
        PrintError(std::cout, status);
    }
    return status;
}

void CameraSDK::PrintPixelFormat(GX_DEV_HANDLE &hDevice)
{
    uint32_t nEntryNums = 0;
    GXGetEnumEntryNums(hDevice, GX_ENUM_PIXEL_FORMAT, &nEntryNums);
    GX_ENUM_DESCRIPTION* enumDescrption = new GX_ENUM_DESCRIPTION[nEntryNums];
    size_t nBufferSize = nEntryNums * sizeof(GX_ENUM_DESCRIPTION);
    GXGetEnumDescription(hDevice, GX_ENUM_PIXEL_FORMAT, enumDescrption, &nBufferSize);
    for (size_t i = 0; i < nEntryNums; ++i)
    {
        std::cout<<enumDescrption[i].szSymbolic<<std::endl;
    }
    delete [] enumDescrption;
}

void CameraSDK::PrintBaseInfo()
{
    for (size_t i = 0; i <nDeviceNum_; ++i)
    {
        std::cout << pBaseinfo_[i].szVendorName << std::endl;
        std::cout << pBaseinfo_[i].szModelName << std::endl;
        std::cout << pBaseinfo_[i].szSN << std::endl;
        std::cout << pBaseinfo_[i].szDisplayName << std::endl;
        std::cout << pBaseinfo_[i].szUserID << std::endl;
    }
}

void CameraSDK::PrintIPInfo()
{
    for (size_t i = 0; i < nDeviceNum_; ++i)
    {
        std::cout << pIPinfo_[i].szDeviceID << std::endl;
        std::cout << pIPinfo_[i].szMAC << std::endl;
        std::cout << pIPinfo_[i].szIP << std::endl;
        std::cout << pIPinfo_[i].szSubNetMask << std::endl;
        std::cout << pIPinfo_[i].szGateWay << std::endl;
    }
}

//  void save_binary(const std::string& prefix_name, const void* data, size_t size, bool with_time_suffix)
//   {
//       time_t current_time = time(nullptr);
//       std::string file_name = prefix_name;
//       if (with_time_suffix) {
//           file_name = file_name + "." + std::to_string(current_time) + ".bin";
//       } else {
//           file_name = file_name + ".bin";
//       }
//       std::fstream fd { file_name, fd.binary | fd.trunc | fd.out };
//       fd.write(reinterpret_cast<char*>(const_cast<void*>(data)), size);
//       fd.close();
//   }

bool isok = false;
void *buf = malloc(2448*2048);

static void GX_STDC OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
    isok = true;
    std::cout<<"on call back func"<<std::endl;
    if (pFrame->status == 0)
    {
        stbi_write_jpg("./2448_2048.jpg", 2448, 2048, 1,pFrame->pImgBuf, 100);
    }
    return;
}


// int main(int argc, char **argv)
// {
//     //  // 获取摄像头信息
//         CameraSDK sdk;
//         sdk.GetDeviceBaseInfo();
//         sdk.PrintBaseInfo();
//     //  // IP配置测试
//         sdk.GetDeviceIPInfo();
//         sdk.PrintIPInfo();
//         // const char *MAC = (sdk.GetAllDeviceIPInfo())[0].szMAC;
//         // std::cout<<sdk.GetDeviceNum()<<std::endl;
//         // sdk.IPConfiguration(const_cast<char*>(MAC), "172.20.30.148", "255.255.255.0", "172.20.30.1", "DaHeng", GX_IP_CONFIGURE_STATIC_IP);
//         // sleep(4);
//         // sdk.GetDeviceIPInfo();
//         // sleep(10);
//         // std::cout<< (sdk.GetAllDeviceIPInfo())[0].szIP<<std::endl;
//         // sdk.IPConfiguration(const_cast<char*>(MAC), nullptr, nullptr, nullptr, nullptr);
//         // sdk.PrintIPInfo();
//         // sdk.GetDeviceIPInfo();
//         // std::cout<< (sdk.GetAllDeviceIPInfo())[0].szIP<<std::endl;
//     //  // 打开设备测试
//         sdk.OpenDevice(1);
//         GX_DEV_HANDLE hDevice =sdk.GetHandle(1);
//         if (!hDevice)
//         {
//             return  0;
//         }
//     //  // 取一帧图测试
//         // sdk.EnableTriggerOnLine(hDevice, 0);
//         sdk.DisableTriggerOnLine(hDevice);
//         int64_t bandwidth = 0;
//         sdk.GetEstimatedBandwidth(hDevice, &bandwidth);
//         std::cout << bandwidth << std::endl;
//         sdk.SetExposureTime(hDevice, 20, 200000);
//         int64_t width = 0;
//         // sdk.SetTestPattern(hDevice, 0);
//         sdk.PrintPixelFormat(hDevice);
//         // sdk.SetROI(hDevice, 0, 0, 2448, 2048);
//         GX_FRAME_DATA frame;
//         frame.pImgBuf = malloc(2448*2048);
//         sdk.StartGrabbing(hDevice);
//         for (size_t i = 0; i < 10; ++i)
//         {
//             sdk.GetOneFrameTimeOut(hDevice, &frame, 10000);
//         }
//          
//         stbi_write_jpg("./2448_2048.jpg", frame.nWidth, frame.nHeight, 1,frame.pImgBuf, 100);
//         sdk.StopGrabbing(hDevice);
//         std::cout << frame.nWidth << " " << frame.nHeight << std::endl;
//         free(frame.pImgBuf);
//         sdk.CloseDevice(hDevice);
//
//         // GX_FRAME_CALLBACK_PARAM pFrame;
//         // sdk.RegisterCallBack(handle, NULL, OnFrameCallbackFun);
//         // GXSendCommand(handle, GX_COMMAND_ACQUISITION_START);
//         // while(!isok)
//         // {
//         //     sleep(3);
//         // }
//         // GXSendCommand(handle, GX_COMMAND_ACQUISITION_STOP);
//         // sdk.UnregisterCallBack(handle);
//     //  // 取一帧图测试
//         // CameraSDK sdk;
//         // sdk.OpenDevice(1);
//         // GX_DEV_HANDLE hDevice = sdk.GetHandle(1);
//         // sdk.PrintPixelFormat(hDevice);
//         // GX_FRAME_DATA frame;
//         // frame.pImgBuf = malloc(2448*2048);
//         // int64_t nPixelFormat = GX_PIXEL_FORMAT_RGB8;
//         // GXSetEnum(handle,  GX_ENUM_PIXEL_FORMAT, nPixelFormat);
//         // sdk.StartGrabbing(handle);
//         // sdk.GetOneFrameTimeOut(handle, &frame, 10000);
//         // sdk.StopGrabbing(handle);
//         // stbi_write_jpg("./2448_2048.jpg", 2448/4, 2048/4, 3,frame.pImgBuf, 100);
//         // void *pRGB = malloc(2448*2048*3);
//         // memset(pRGB, 0, 2448*2048*3);
//         // DxRaw8toRGB24(frame.pImgBuf, pRGB, 2448, 2048, RAW2RGB_NEIGHBOUR, BAYERBG, true);
//         // stbi_write_jpg("./2448_2048rgb.jpg", 2448, 2048, 3,pRGB, 100);
//         // free(frame.pImgBuf);
//         // sdk.CloseDevice(handle);
//         // free(pRGB);
//     // DeviceInfomation
//     // CameraSDK sdk;
//     // sdk.GetDeviceBaseInfo();
//     // const GX_DEVICE_BASE_INFO *p = sdk.GetDeviceInfo();
//     // std::cout<<p->szSN<<std::endl;
//     // std::cout<<p->szUserID<<std::endl;
//     // if(sdk.OpenDevice(,const_cast<char*>(p->szSN)))
//     // {
//     //     std::cout<<"opend"<<std::endl;
//     // }
//     // if(sdk.OpenDevice(GX_OPEN_IP, "172.20.30.142"))
//     // {
//     //     std::cout<<"opend"<<std::endl;
//     // };
//
//     // // 获取Lib版本号
//     // const char *pLibVersion = GXGetLibVersion();
//     // std::cout<<pLibVersion<<std::endl;
//     //
//     // GX_STATUS status = GX_STATUS_SUCCESS;
//     //
//     // status = GXInitLib();
//     //
//     // if (status != GX_STATUS_SUCCESS)
//     // {
//     //    return 0;
//     // }
//     //
//     // // 枚举相机信息
//     // uint32_t nDeviceNum = 0;
//     // status = GXUpdateDeviceList(&nDeviceNum, 1000);
//     // if (nDeviceNum == 0)
//     // {
//     //     std::cout<<"no camera online"<<std::endl;
//     // }
//     // if (status == GX_STATUS_SUCCESS&&nDeviceNum > 0)
//     // {
//     //    GX_DEVICE_BASE_INFO *pBaseinfo = new GX_DEVICE_BASE_INFO[nDeviceNum];
//     //    size_t nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);
//     //
//     //    // 获取所有设备的基础信息
//     //     status = GXGetAllDeviceBaseInfo(pBaseinfo, &nSize);
//     //     delete []pBaseinfo;
//     // }
//     // // 结束使用释放资源
//     // status = GXCloseLib();
//     // CameraSDK sdk;
//     // std::cout<<sdk.GetLibVersion()<<std::endl;
//     // GX_DEV_HANDLE hDevice;
//     // Log(std::cout, "current deviceNum: ");
//     // std::cout<<sdk.GetDeviceList()<<std::endl;
//     // sdk.GetDeviceBaseInfo();
//     // sdk.OpenDevice(1);
//     // int64_t nPayLoadSize;
//     //  GXGetInt(hDevice, GX_INT_PAYLOAD_SIZE, &nPayLoadSize);
//     //  sdk.CloseDevice(hDevice);
//     //    //  void *buff = malloc((size_t)nPayLoadSize);
//     //      GX_FRAME_DATA f;
//     //      f.pImgBuf = malloc((size_t)nPayLoadSize);
//     //      sdk.setIndex(1);
//     //      sdk.GetOneFrameTimeOut(hDevice, 10000, &f, f.pImgBuf);
//     //      std::cout<<"img-size"<<f.nImgSize<<"pixel"<<f.nPixelFormat<<"IMG h"<<f.nHeight<<"IMG w"<<f.nWidth<<std::endl;
//     //      stbi_write_jpg("./2448_2048.jpg", 2448, 2048, 1, f.pImgBuf, 100);
//          //
//     // for (size_t i = 0; i < 10; ++i)
//     // {
//     //     Log(std::cout, "loop");
//     //     if(sdk.OpenDevice(1, hDevice))
//     //     {
//     //         Log(std::cout, "opened");
//     //     }
//     //     sleep(2);
//     //     Log(std::cout, "sleep done");
//     //     if(sdk.OpenDevice(hDevice))
//     //     {
//     //         Log(std::cout, "closed");
//     //     }
//     // }
//     // 取图测试
//         // CameraSDK sdk;
//         // sdk.GetDeviceIPInfo();
//         //  sdk.OpenDevice(GX_OPEN_IP, "172.20.30.148");
//         // const std::vector<OpenParam> &o = sdk.GetOpenedList();
//         // GX_DEV_HANDLE hDevice = o[0].GetHandle();
//         // int64_t nPayLoadSize;
//         // GXGetInt(hDevice, GX_INT_PAYLOAD_SIZE, &nPayLoadSize);
//         // GX_FRAME_DATA f;
//         // f.pImgBuf = malloc((size_t)nPayLoadSize);
//         // sdk.GetOneFrameTimeOut(hDevice, 10000, &f, 0);
//         //  std::cout<<"img-size"<<f.nImgSize<<"pixel"<<f.nPixelFormat<<"IMG h"<<f.nHeight<<"IMG w"<<f.nWidth<<std::endl;
//         //  stbi_write_jpg("./2448_2048.jpg", 2448, 2048, 1, f.pImgBuf, 100);
//         //  free(f.pImgBuf);
//
//     // else
//     // {
//     //     std::cout<<"open failed"<<std::endl;
//     // }
//     // if (sdk.CloseDevice(hDevice))
//     // {
//     //     std::cout<<"close succeed"<<std::endl;
//     // }
//     // sdk.GetOneFrameTimeOut(hDevice, 10000, nullptr);
//     // std::cout<<sdk.getP()->nBufID<<std::endl;
//     return 0;
// }
