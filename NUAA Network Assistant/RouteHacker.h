#pragma once

#define ROUTE_PERSISTENT_REG _T("SYSTEM\\CurrentControlSet\\Services\\TcpIp\\Parameters\\PersistentRoutes")

class RouteHacker
{
private:
	NetInfo netInfo;
	BOOL isRouteHacked;
public:
	static DWORD AddEntry(NetInfo adapter, CString dest, CString mask, BOOL persistent);
	static DWORD DeleteEntry(NetInfo adapter, CString dest, CString mask);

	RouteHacker();
	~RouteHacker();

	BOOL Init(NetInfo & adapter);
	BOOL FlushRoute();
	BOOL HackRoute(BOOL persistent);
	BOOL IsRouteHacked();
};
