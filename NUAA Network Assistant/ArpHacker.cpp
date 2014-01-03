#include "stdafx.h"

ArpHacker::ArpHacker():
isArpHacked(FALSE)
{
}


ArpHacker::~ArpHacker()
{
}

BOOL ArpHacker::IsArpHacked(){
	return isArpHacked;
}

BOOL ArpHacker::Init(NetInfo & adapter)
{
	if (isArpHacked)FlushArp();

	netInfo = adapter;

	return TRUE;
}

BOOL ArpHacker::FlushArp()
{
	isArpHacked = FALSE;

	DWORD dwRetVal = FlushIpNetTable(netInfo.dAdapterIndex);
	if (dwRetVal == NO_ERROR){

		return TRUE;
	}

	return FALSE;
}

BOOL ArpHacker::HackArp()
{
	if (isArpHacked)return TRUE;

	// Step 2: 添加本地 ARP 缓存
	// 2.1: 计算局域网 ip 段

	DWORD num_ip = 1;
	DWORD mask_t = ~SwapEndian(netInfo.localMask);
	int mask_length = 0;
	for (int i = 0; i < 32; i++)
	{
		if (mask_t & 1)
			++mask_length;
		else
			break;
		mask_t >>= 1;
	}
	if (mask_length > 10)
	{
		// Something must be wrong
		mask_length = 8;
	}
	for (int i = 0; i < mask_length; i++)
		num_ip <<= 1;
	DWORD ip_start = (SwapEndian(netInfo.localIp) & SwapEndian(netInfo.localMask)) + 1;
	DWORD ip_end = (SwapEndian(netInfo.localIp) & SwapEndian(netInfo.localMask)) + num_ip - 1;

	// 2.2: 先清空一下缓存
	FlushArp();

	// 2.3: 添加到缓存

	DWORD ip;
	MIB_IPNETROW ipnetrow;
	memset(&ipnetrow, 0, sizeof(MIB_IPNETROW));
	ipnetrow.dwIndex = netInfo.dAdapterIndex;
	ipnetrow.dwPhysAddrLen = 6;
	memcpy(ipnetrow.bPhysAddr, netInfo.gatewayMac, 6);
	ipnetrow.dwType = MIB_IPNET_TYPE_STATIC;

	DWORD dwRetVal;

	for (DWORD _ip = ip_start; _ip < ip_end; _ip++)
	{
		ip = SwapEndian(_ip);
		if (ip == netInfo.localIp || ip == netInfo.gatewayIp)
			continue;
		ipnetrow.dwAddr = ip;
		dwRetVal = CreateIpNetEntry(&ipnetrow);
		if (dwRetVal != NO_ERROR)
		{
			MessageBox(NULL, _T("ARP 缓存添加失败"), NULL, MB_ICONERROR);
			FlushArp();
			return FALSE;
		}
	}

	isArpHacked = TRUE;

	return TRUE;
}
