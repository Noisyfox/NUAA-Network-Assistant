#pragma once

#include <thread>

//屏蔽TyDial中引用的winsock
#define _WINSOCKAPI_
#include "TyDial.hpp"

#define WM_RASDIAL_UPDATE WM_USER + 103L

class TianyiDial
{
private:
	std::thread * m_service_thread;
	TyDial m_tydial;
	HRASCONN hRasConn;

	static HWND m_msg_hwnd;
	static void WINAPI RasDialCallback(UINT unMsg, RASCONNSTATE rasconnstat, DWORD dwError);

	friend void DialThread(TianyiDial* dial, CString account, CString passwd);
public:
	TianyiDial();
	~TianyiDial();

	static void RegisterMessage(HWND hWnd);

	void OnCallback(RASCONNSTATE rasconnstat, DWORD dwError);

	BOOL Dial(NetInfo adapter, CString account, CString passwd);
	BOOL Disconnect();
	void Clean();

	BOOL IsConnected();
};

