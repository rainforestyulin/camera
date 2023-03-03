typedef enum GX_STATUS_LIST
{
    GX_STATUS_SUCCESS                   =  0,          ///< Success
    GX_STATUS_ERROR                     = -1,          ///< There is an unspecified internal error that is not expected to occur
    GX_STATUS_NOT_FOUND_TL              = -2,          ///< The TL library cannot be found
    GX_STATUS_NOT_FOUND_DEVICE          = -3,          ///< The device is not found
    GX_STATUS_OFFLINE                   = -4,          ///< The current device is in an offline status
    GX_STATUS_INVALID_PARAMETER         = -5,          ///< Invalid parameter. Generally, the pointer is NULL or the input IP and other parameter formats are invalid
    GX_STATUS_INVALID_HANDLE            = -6,          ///< Invalid handle
    GX_STATUS_INVALID_CALL              = -7,          ///< The interface is invalid, which refers to software interface logic error
    GX_STATUS_INVALID_ACCESS            = -8,          ///< The function is currently inaccessible or the device access mode is incorrect
    GX_STATUS_NEED_MORE_BUFFER          = -9,          ///< The user request buffer is insufficient: the user input buffer size during the read operation is less than the actual need
    GX_STATUS_ERROR_TYPE                = -10,         ///< The type of FeatureID used by the user is incorrect, such as an integer interface using a floating-point function code
    GX_STATUS_OUT_OF_RANGE              = -11,         ///< The value written by the user is crossed
    GX_STATUS_NOT_IMPLEMENTED           = -12,         ///< This function is not currently supported
    GX_STATUS_NOT_INIT_API              = -13,         ///< There is no call to initialize the interface
    GX_STATUS_TIMEOUT                   = -14,         ///< Timeout error
}GX_STATUS_LIST;

