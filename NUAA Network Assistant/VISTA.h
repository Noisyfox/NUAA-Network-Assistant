#pragma once

//增加部分自vista才会有的函数和结构体

#if _WIN32_WINNT < _WIN32_WINNT_VISTA

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

DWORD ConvertIpv4MaskToLength(
__in ULONG Mask,
__out PUINT8 MaskLength
);

#endif
