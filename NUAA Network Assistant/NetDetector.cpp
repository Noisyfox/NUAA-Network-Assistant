#include "stdafx.h"

NetDetector::NetDetector() :
pAdapterInfo(NULL), pSelectedAdapter(NULL)
{
}

NetDetector::~NetDetector()
{
	if (!pAdapterInfo){
		free(pAdapterInfo);
		pAdapterInfo = NULL;
	}
	pSelectedAdapter = NULL;
}

BOOL NetDetector::ListAllAdapter(PIP_ADAPTER_INFO & adapterInfo, BOOL forceRescan)
{
	if (pAdapterInfo){
		if (forceRescan){
			free(pAdapterInfo);
			pAdapterInfo = NULL;
		}
		else {
			adapterInfo = pAdapterInfo;
			return TRUE;
		}
	}

	pSelectedAdapter = NULL;

	PIP_ADAPTER_INFO pAdapter = NULL;
	ULONG outBufLen = 0;
	ULONG dwRetVal;

	// 读取网络适配器信息

	for (int retry = 0; retry < 3; retry++)
	{
		pAdapter = (PIP_ADAPTER_INFO)malloc(outBufLen);
		dwRetVal = GetAdaptersInfo(pAdapter, &outBufLen);
		if (dwRetVal == NO_ERROR)
			break;
		if (dwRetVal == ERROR_BUFFER_OVERFLOW)
		{
			free(pAdapter);
			pAdapter = NULL;
		}
	}
	if (dwRetVal != NO_ERROR)
		return FALSE;

	pAdapterInfo = pAdapter;
	adapterInfo = pAdapterInfo;

	return TRUE;
}

BOOL NetDetector::AutoSetAdapter(CString defaultAdapter)
{
	DWORD mask, gateway;
	gateway = inet_addr("172.19.0.0");
	mask = inet_addr("255.255.0.0");

	PIP_ADAPTER_INFO pAdapter, _tmpAdapter;
	if (!ListAllAdapter(pAdapter, FALSE)){
		return FALSE;
	}
	
	defaultAdapter.ReleaseBuffer();
	BOOL hasDefaultAdapter = !defaultAdapter.IsEmpty();

	if (hasDefaultAdapter)
	{
		_tmpAdapter = pAdapter;
		// 枚举所有适配器
		while (_tmpAdapter)
		{
			if (CString(_tmpAdapter->AdapterName) == defaultAdapter)
			{
				// 保存信息
				return SetAdapter(_tmpAdapter);
			}
			_tmpAdapter = _tmpAdapter->Next;
		}
	}

	//没有找到默认的适配器或者没有设置默认的适配器
	_tmpAdapter = pAdapter;
	// 枚举所有适配器
	while (_tmpAdapter)
	{
		IPAddr ip = inet_addr(_tmpAdapter->GatewayList.IpAddress.String);
		BYTE * addr = (BYTE*)&ip;

		if ((ip & mask) == (gateway & mask))
		{
			// 保存信息
			return SetAdapter(_tmpAdapter);
		}
		_tmpAdapter = _tmpAdapter->Next;
	}

	return FALSE;
}

BOOL NetDetector::SetAdapter(PIP_ADAPTER_INFO adapter)
{
	pSelectedAdapter = adapter;

	return TRUE;
}

BOOL NetDetector::ObtainInformation(NetInfo & info)
{
	if (!pSelectedAdapter)return FALSE;

	info.dAdapterIndex = pSelectedAdapter->Index;
	info.localIp = inet_addr(pSelectedAdapter->IpAddressList.IpAddress.String);
	info.localMask = inet_addr(pSelectedAdapter->IpAddressList.IpMask.String);
	info.gatewayIp = inet_addr(pSelectedAdapter->GatewayList.IpAddress.String);
	info.sLocalIp = pSelectedAdapter->IpAddressList.IpAddress.String;
	info.sLocalMask = pSelectedAdapter->IpAddressList.IpMask.String;
	info.sGatewayIp = pSelectedAdapter->GatewayList.IpAddress.String;
	info.sAdapterName = pSelectedAdapter->AdapterName;
	info.sAdapterDesc = pSelectedAdapter->Description;
	memcpy(info.localMac, pSelectedAdapter->Address, 6);
	info.sLocalMac.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),
		pSelectedAdapter->Address[0],
		pSelectedAdapter->Address[1],
		pSelectedAdapter->Address[2],
		pSelectedAdapter->Address[3],
		pSelectedAdapter->Address[4],
		pSelectedAdapter->Address[5]);

	//解析网关的 MAC 地址
	DWORD dwRetVal;
	//ULONG macaddr[2];
	ULONG outBufLen = 6;
	dwRetVal = SendARP(info.gatewayIp, 0, info.gatewayMac, &outBufLen);

	if (dwRetVal != NO_ERROR)
	{
		MessageBox(NULL, _T("解析网关 MAC 地址失败"), NULL, MB_ICONERROR);
		return FALSE;
	}

	BYTE * addr = (BYTE*)info.gatewayMac;
	info.sGatewayMac.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"), addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

	return TRUE;
}
