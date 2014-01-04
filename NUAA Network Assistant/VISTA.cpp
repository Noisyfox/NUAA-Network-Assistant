#include "stdafx.h"
#include "VISTA.h"

DWORD ConvertIpv4MaskToLength(
	__in ULONG Mask,
	__out PUINT8 MaskLength
	)
{
	DWORD mask = SwapEndian(Mask);
	UINT8 len = 0;
	while (mask != 0 && (mask & 1) == 0){
		mask >>= 1;
		len++;
	}

	while (mask != 0){
		if ((mask & 1) == 0)return ERROR_INVALID_PARAMETER;
		mask >>= 1;
	}

	*MaskLength = 32 - len;

	return NO_ERROR;
}
