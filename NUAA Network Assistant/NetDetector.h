#pragma once

#include <atlstr.h>

typedef struct {
	CString sAdapterDesc, sAdapterName;
	CString sLocalIp, sLocalMask, sLocalMac;
	CString sGatewayIp, sGatewayMac;

	DWORD dAdapterIndex;
	IPAddr localIp, gatewayIp;
	IPMask localMask;
	BYTE localMac[6], gatewayMac[6];
}NetInfo;

class NetDetector
{
private:
	PIP_ADAPTER_INFO pAdapterInfo, pSelectedAdapter;
public:
	NetDetector();
	~NetDetector();

	BOOL ListAllAdapter(PIP_ADAPTER_INFO & adapterInfo, BOOL forceRescan);
	BOOL AutoSetAdapter();
	BOOL SetAdapter(PIP_ADAPTER_INFO adapter);
	//BOOL SetAdapter(DWORD index);
	BOOL ObtainInformation(NetInfo & info);
};
