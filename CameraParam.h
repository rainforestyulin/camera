#include "GxIAPI.h"
#include <stdint.h>
//
// typedef struct _DH_FRAME_OUT_INFO_
// {
//     unsigned short nWidth;
//     unsigned short nHeight;
//
//     unsigned int nFrameNum;
//     unsigned int nDevTimeStampHigh;
//     unsigned int nDevTimeStampLow;
//     unsigned int nReserved0;
//     int64_t  nHostTimeStamp;
//
//     unsigned int nFrameLen;
//
//     unsigned int nLostPacket;
//     unsigned int nReserved[2];
// }DH_FRAME_OUT_INFO;
//
// typedef struct _DH_GIGE_DEVICE_INFO_
// {
//     unsigned int        nIpCfgOption;
//     unsigned int        nIpCfgCurrent;
//     unsigned int        nCurrentIp;
//     unsigned int        nCurrentSubNetMask;
//     unsigned int        nDefultGateWay;
//     unsigned char       chManufacturerName[32];
//     unsigned char       chModelName[32];
//     unsigned char       chDeviceVersion[32];
//     unsigned char       chManufacturerSpecificInfo[48];
//     unsigned char       chSerialNumber[16];
//     unsigned char       chUserDefinedName[16];
//
//     unsigned int        nNetExport;
//
//     unsigned int        nReserved[4];
//
// }DH_GIGE_DEVICE_INFO;
// #define GX_INFO_LENGTH_8_BYTE   (8)  ///< 8 bytes
// #define GX_INFO_LENGTH_32_BYTE  (32) ///< 32 bytes
// #define GX_INFO_LENGTH_64_BYTE  (64) ///< 64 bytes
// #define GX_INFO_LENGTH_128_BYTE (128)///< 128 bytes
//
// typedef struct GX_CC_DEVICE_IP_INFO 
// {
//     char szDeviceID[GX_INFO_LENGTH_64_BYTE + 4];         ///< the unique identifier of the device, 64+4 bytes
//     char szMAC[GX_INFO_LENGTH_32_BYTE];                  ///< MAC address, 32 bytes,
//     char szIP[GX_INFO_LENGTH_32_BYTE];                   ///< IP address, 32 bytes,
//     char szSubNetMask[GX_INFO_LENGTH_32_BYTE];           ///< subnet mask, 32 bytes
//     char szGateWay[GX_INFO_LENGTH_32_BYTE];              ///< gateway, 32 bytes
//     char szNICMAC[GX_INFO_LENGTH_32_BYTE];               ///< the MAC address of the corresponding NIC(Network Interface Card), 32 bytes
//     char szNICIP[GX_INFO_LENGTH_32_BYTE];                ///< the IP address of the corresponding NIC, 32 bytes
//     char szNICSubNetMask[GX_INFO_LENGTH_32_BYTE];        ///< the subnet mask of the corresponding NIC, 32 bytes
//     char szNICGateWay[GX_INFO_LENGTH_32_BYTE];           ///< the gateway of the corresponding NIC, 32 bytes
//     char szNICDescription[GX_INFO_LENGTH_128_BYTE + 4];  ///< the description of the corresponding NIC, 128+4 bytes
//     char reserved[512];                                  ///< reserved, 512 bytes
// }GX_CC_DEVICE_IP_INFO;
//
// //------------------------------------------------------------------------------
// //  Device Type Definition
// //------------------------------------------------------------------------------
// typedef enum GX_CC_DEVICE_CLASS_LIST
// {
//     GX_DEVICE_CLASS_UNKNOWN             = 0,           ///< Unknown device type
//     GX_DEVICE_CLASS_USB2                = 1,           ///< USB2.0 Vision device
//     GX_DEVICE_CLASS_GEV                 = 2,           ///< Gige Vision device
//     GX_DEVICE_CLASS_U3V                 = 3,           ///< USB3 Vision device
//     GX_DEVICE_CLASS_SMART               = 4,           ///< Smart camera device
// }GX_CC_DEVICE_CLASS_LIST;
// typedef  int32_t GX_CC_DEVICE_CLASS;
// //------------------------------------------------------------------------------
// //  Access Status of Current Device
// //------------------------------------------------------------------------------
// typedef enum GX_CC_ACCESS_STATUS
// {
//     GX_ACCESS_STATUS_UNKNOWN            = 0,           ///< The device's current status is unknown
//     GX_ACCESS_STATUS_READWRITE          = 1,           ///< The device currently supports reading and writing
//     GX_ACCESS_STATUS_READONLY           = 2,           ///< The device currently only supports reading
//     GX_ACCESS_STATUS_NOACCESS           = 3,           ///< The device currently does neither support reading nor support writing
// }GX_CC_ACCESS_STATUS;
// typedef int32_t GX_CC_ACCESS_STATUS_CMD;
//
// typedef struct _GX_CC_DEVICE_BASE_INFO_
// {
//     char szVendorName[GX_INFO_LENGTH_32_BYTE];           ///< vendor name, 32 bytes
//     char szModelName[GX_INFO_LENGTH_32_BYTE];            ///< model name, 32 bytes
//     char szSN[GX_INFO_LENGTH_32_BYTE];                   ///< device serial number, 32 bytes
//     char szDisplayName[GX_INFO_LENGTH_128_BYTE + 4];     ///< device display name, 128+4 bytes
//     char szDeviceID[GX_INFO_LENGTH_64_BYTE + 4];         ///< the unique identifier of the device, 64+4 bytes
//     char szUserID[GX_INFO_LENGTH_64_BYTE + 4];           ///< user-defined name, 64+4 bytes
//     GX_CC_ACCESS_STATUS_CMD accessStatus;                   ///< access status that is currently supported by the device. Refer to GX_ACCESS_STATUS, 4 bytes
//     GX_CC_DEVICE_CLASS      deviceClass;                    ///< device type, such as USB2.0, GEV, 4 bytes
//     char reserved[12];                                   ///< reserved, 12 bytes
// }GX_CC_DEVICE_BASE_INFO;
//
typedef struct _GX_CC_DEVICE_INFO_
{
    uint32_t device_index;                ///< device index
    GX_DEVICE_BASE_INFO device_base_info; ///< device base information
    GX_DEVICE_IP_INFO device_ip_info;     ///< device ip information
}GX_CC_DEVICE_INFO;

#define DH_MAX_DEVICE_NUM      256
typedef struct _DH_CC_DEVICE_INFO_LIST_
{
    uint32_t                device_num;
    GX_CC_DEVICE_INFO*      device_info[DH_MAX_DEVICE_NUM];

}GX_CC_DEVICE_INFO_LIST;



