#include "DHCameraControl.h"
#include <stdio.h>
void GX_CC_InitLib();
const char* GX_CC_GetSDKVersion();
int main()
{
    GX_CC_InitLib();
    printf(GX_CC_GetSDKVersion());
    return 0;
}
