#pragma once

#include <thread>

//屏蔽TyDial中引用的winsock
#define _WINSOCKAPI_
#include "TyDial.hpp"

#define WM_RASDIAL_UPDATE WM_USER + 103L
#define CHAP_PREFIX (_T("^#02"))

//typedef BOOL(TianyiDial::*DIALMODE)(NetInfo, CString, CString);

class TianyiDial
{
private:
	std::thread * m_service_thread;
	TyDial m_tydial;
	HRASCONN hRasConn;
	BOOL(TianyiDial::*m_dial_mode)(NetInfo, CString, CString);

	static HWND m_msg_hwnd;
	static void WINAPI RasDialCallback(UINT unMsg, RASCONNSTATE rasconnstat, DWORD dwError);
	static BOOL CreatePPPOE(BOOL chap);

	BOOL DialMode_Emulate(NetInfo adapter, CString account, CString passwd);

	friend void DialThread(TianyiDial* dial);
public:
	TianyiDial();
	~TianyiDial();

	static void RegisterMessage(HWND hWnd);

	void OnCallback(RASCONNSTATE rasconnstat, DWORD dwError);

	BOOL Dial(NetInfo adapter, CString account, CString passwd);
	BOOL SetDialMode(int mode);
	int GetDialModeCount(){ return 1; }
	BOOL Disconnect();
	void Clean();

	BOOL IsConnected();
};

