#include "stdafx.h"
#include "RouteHacker.h"

#ifndef _WS2IPDEF_ 
//解决 sdkver < vista 时没有该结构体定义
//
// The MIB structure for Network layer Interface management routines.
//
typedef struct _MIB_IPINTERFACE_ROW {
	//
	// Key Structure;
	//
	ADDRESS_FAMILY Family;
	NET_LUID InterfaceLuid;
	NET_IFINDEX InterfaceIndex;

	//
	// Read-Write fields.
	//

	//
	// Fields currently not exposed.
	//
	ULONG MaxReassemblySize;
	ULONG64 InterfaceIdentifier;
	ULONG MinRouterAdvertisementInterval;
	ULONG MaxRouterAdvertisementInterval;

	//
	// Fileds currently exposed.
	//       
	BOOLEAN AdvertisingEnabled;
	BOOLEAN ForwardingEnabled;
	BOOLEAN WeakHostSend;
	BOOLEAN WeakHostReceive;
	BOOLEAN UseAutomaticMetric;
	BOOLEAN UseNeighborUnreachabilityDetection;
	BOOLEAN ManagedAddressConfigurationSupported;
	BOOLEAN OtherStatefulConfigurationSupported;
	BOOLEAN AdvertiseDefaultRoute;

	NL_ROUTER_DISCOVERY_BEHAVIOR RouterDiscoveryBehavior;
	ULONG DadTransmits;         // DupAddrDetectTransmits in RFC 2462.    
	ULONG BaseReachableTime;
	ULONG RetransmitTime;
	ULONG PathMtuDiscoveryTimeout; // Path MTU discovery timeout (in ms).

	NL_LINK_LOCAL_ADDRESS_BEHAVIOR LinkLocalAddressBehavior;
	ULONG LinkLocalAddressTimeout; // In ms.
	ULONG ZoneIndices[ScopeLevelCount]; // Zone part of a SCOPE_ID.
	ULONG SitePrefixLength;
	ULONG Metric;
	ULONG NlMtu;

	//
	// Read Only fields.
	//
	BOOLEAN Connected;
	BOOLEAN SupportsWakeUpPatterns;
	BOOLEAN SupportsNeighborDiscovery;
	BOOLEAN SupportsRouterDiscovery;

	ULONG ReachableTime;

	NL_INTERFACE_OFFLOAD_ROD TransmitOffload;
	NL_INTERFACE_OFFLOAD_ROD ReceiveOffload;

	//
	// Disables using default route on the interface. This flag
	// can be used by VPN clients to restrict Split tunnelling.
	//
	BOOLEAN DisableDefaultRoutes;
} MIB_IPINTERFACE_ROW, *PMIB_IPINTERFACE_ROW;
#endif // _WS2IPDEF_

DWORD GetMetric(NetInfo adapter)
{
	OSVERSIONINFO ver;

	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (0 == ::GetVersionEx(&ver))return -1L;

	if (ver.dwMajorVersion > 5)/////////////////如果版本在XP以后（win7,vista....)
	{
		typedef DWORD(__stdcall *IPINTENTRY)(PMIB_IPINTERFACE_ROW);
		MIB_IPINTERFACE_ROW info;
		info.InterfaceIndex = adapter.dAdapterIndex;
		memset(&(info.InterfaceLuid), 0, sizeof(info.InterfaceLuid));
		info.Family = AF_INET;
		HINSTANCE hInst = ::LoadLibrary(_T("Iphlpapi.dll"));
		if (NULL == hInst)
			return -1L;

		IPINTENTRY pFunGetInfEntry = (IPINTENTRY)GetProcAddress(hInst, "GetIpInterfaceEntry");

		DWORD dwRet = pFunGetInfEntry(&info);

		::FreeLibrary(hInst);

		if (NO_ERROR != dwRet)
			return -1L;

		return info.Metric + 1L;
	}

	return 1L;
}

DWORD CampusNetCount = 2;
CString CampusNet[][2] ={ 
	{ _T("211.65.96.0"), _T("255.255.240.0") },
	{ _T("202.119.64.0"), _T("255.255.240.0") } 
};

RouteHacker::RouteHacker():
isRouteHacked(FALSE)
{
}


RouteHacker::~RouteHacker()
{
}


BOOL RouteHacker::Init(NetInfo & adapter)
{
	if (isRouteHacked)FlushRoute();

	netInfo = adapter;

	return TRUE;
}

BOOL RouteHacker::FlushRoute()
{
	isRouteHacked = FALSE;

	BOOL fail = FALSE;

	for (DWORD i = 0; i < CampusNetCount; i++){
		DWORD dwRetVal = DeleteEntry(netInfo, CampusNet[i][0], CampusNet[i][1]);
		if (dwRetVal != ERROR_NOT_FOUND && dwRetVal != NO_ERROR){
			fail = TRUE;
		}
	}

	return !fail;
}

BOOL RouteHacker::HackRoute(BOOL persistent)
{
	if (isRouteHacked)return TRUE;

	FlushRoute();

	for (DWORD i = 0; i < CampusNetCount; i++){
		DWORD dwRetVal = AddEntry(netInfo, CampusNet[i][0], CampusNet[i][1], persistent);
		if (dwRetVal != ERROR_OBJECT_ALREADY_EXISTS && dwRetVal != NO_ERROR){
			FlushRoute();
			return FALSE;
		}
	}

	isRouteHacked = TRUE;

	return TRUE;
}

BOOL RouteHacker::IsRouteHacked()
{
	return isRouteHacked;
}

DWORD RouteHacker::AddEntry(NetInfo adapter, CString dest, CString mask, BOOL persistent)
{
	DWORD iDest, iMask;

	if (!NetAddr(dest, iDest) || !NetAddr(mask, iMask))return FALSE;

	DWORD metric = GetMetric(adapter);
	if (metric < 0) return FALSE;

	MIB_IPFORWARDROW IpForwardTable;
	ZeroMemory(&IpForwardTable, sizeof(MIB_IPFORWARDROW));

	IpForwardTable.dwForwardDest = iDest;
	IpForwardTable.dwForwardMask = iMask;
	IpForwardTable.dwForwardNextHop = adapter.gatewayIp;
	IpForwardTable.dwForwardIfIndex = adapter.dAdapterIndex;
	IpForwardTable.dwForwardType = 4;  //路由类型 3是最终目标，4是非最终目标,我们加的路由都是要过网关的，设成4 
	IpForwardTable.dwForwardProto = 3;  //路由协议，这个在这个函数里要设成3 
	IpForwardTable.dwForwardPolicy = NULL;
	IpForwardTable.dwForwardAge = NULL;
	IpForwardTable.dwForwardNextHopAS = NULL;
	IpForwardTable.dwForwardMetric1 = metric;
	IpForwardTable.dwForwardMetric2 = -1;
	IpForwardTable.dwForwardMetric3 = -1;
	IpForwardTable.dwForwardMetric4 = -1;
	IpForwardTable.dwForwardMetric5 = -1;
	DWORD dwRetVal = CreateIpForwardEntry(&IpForwardTable);

	return dwRetVal;
}

DWORD RouteHacker::DeleteEntry(NetInfo adapter, CString dest, CString mask)
{

	DWORD iDest, iMask;

	if (!NetAddr(dest, iDest) || !NetAddr(mask, iMask))return FALSE;

	MIB_IPFORWARDROW IpForwardTable;
	ZeroMemory(&IpForwardTable, sizeof(MIB_IPFORWARDROW));
	IpForwardTable.dwForwardDest = iDest;
	IpForwardTable.dwForwardMask = iMask;
	IpForwardTable.dwForwardPolicy = NULL;
	IpForwardTable.dwForwardNextHop = adapter.gatewayIp;
	IpForwardTable.dwForwardIfIndex = adapter.dAdapterIndex;
	IpForwardTable.dwForwardType = 4;  //路由类型 3是最终目标，4是非最终目标,我们加的路由都是要过网关的，设成4 
	IpForwardTable.dwForwardProto = 3;  //路由协议，这个在这个函数里要设成3 
	//IpForwardTable.dwForwardAge = NULL;
	//IpForwardTable.dwForwardNextHopAS = NULL;
	IpForwardTable.dwForwardMetric1 = -1;
	IpForwardTable.dwForwardMetric2 = -1;
	IpForwardTable.dwForwardMetric3 = -1;
	IpForwardTable.dwForwardMetric4 = -1;
	IpForwardTable.dwForwardMetric5 = -1;
	DWORD dwRetVal = DeleteIpForwardEntry(&IpForwardTable);

	return dwRetVal;
}
