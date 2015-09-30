#include "GSensor.h"
#include "debug.h"

BOOL GSensor_Init(void)
{
    return TRUE;
}

BOOL GSensor_open(void)
{
    return TRUE;
}

BOOL GSensor_close(void)
{
    return TRUE;
}


BOOL GSensor_GetData(Gsensor_Data *GS_Data)
{
    return TRUE;
}

void GSensor_GetStatus(UINT32 status,UINT32 *uiData)
{

}

BOOL GSensor_ParkingMode(void)
{
    return FALSE;
}

BOOL GSensor_CrashMode(void)
{
    return FALSE;
}

void GSensor_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity)
{

}

