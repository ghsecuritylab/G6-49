#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Hardware/VCP.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
USBD_HandleTypeDef VCP::handleUSBD;
PCD_HandleTypeDef  VCP::handlePCD;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VCP::SendFormatStringAsynch(char *, ...)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendDataSynch(const uint8 *, int)
{

}
