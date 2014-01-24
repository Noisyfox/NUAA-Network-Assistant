#pragma once

#include <thread>

//屏蔽TyDial中引用的winsock
#define _WINSOCKAPI_
#include "TyDial.hpp"

#define WM_RASDIAL_UPDATE WM_USER + 103L
#define PAP_PREFIX (_T("^~2"))
#define CHAP_POSTFIX (_T("@163.js"))

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
	BOOL DialMode_PAP_Prefix(NetInfo adapter, CString account, CString passwd);
	BOOL DialMode_CHAP_Postfix(NetInfo adapter, CString account, CString passwd);

	friend void DialThread(TianyiDial* dial, CString account, CString passwd);
public:
	TianyiDial();
	~TianyiDial();

	static void RegisterMessage(HWND hWnd);

	void OnCallback(RASCONNSTATE rasconnstat, DWORD dwError);

	BOOL Dial(NetInfo adapter, CString account, CString passwd);
	BOOL SetDialMode(int mode);
	int GetDialModeCount(){ return 3; }
	BOOL Disconnect();
	void Clean();

	BOOL IsConnected();
};

