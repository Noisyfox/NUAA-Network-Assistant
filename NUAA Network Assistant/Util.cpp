#include "stdafx.h"
#include "Util.h"

DWORD SwapEndian(DWORD value)
{
	return ((value >> 24) & 0xFF) |
		((value >> 8) & 0xFF00) |
		((value << 8) & 0xFF0000) |
		((value << 24) & 0xFF000000);
}

BOOL NetAddr(CString s, DWORD & i){

	if (s == _T("255.255.255.255")){
		i = -1;
	}
	else {
		USES_CONVERSION;
		DWORD p = inet_addr(T2A(s));
		if (p == INADDR_NONE)return FALSE;
		i = p;
	}

	return TRUE;
}
