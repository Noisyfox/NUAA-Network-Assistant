#include "stdafx.h"
#include "TianyiDial.h"
#include <Ras.h>
#pragma comment(lib, "rasapi32.lib")

HWND TianyiDial::m_msg_hwnd = NULL;

TianyiDial::TianyiDial():
m_dial_mode(&TianyiDial::DialMode_Emulate)
{
}


TianyiDial::~TianyiDial()
{
	Clean();
}

void TianyiDial::RegisterMessage(HWND hWnd)
{
	m_msg_hwnd = hWnd;
}

void WINAPI TianyiDial::RasDialCallback(UINT unMsg, RASCONNSTATE rasconnstat, DWORD dwError)
{

	if (m_msg_hwnd == NULL)return;

	SendNotifyMessage(m_msg_hwnd, WM_RASDIAL_UPDATE, rasconnstat, dwError);

}

void TianyiDial::OnCallback(RASCONNSTATE rasconnstat, DWORD dwError)
{
	if (rasconnstat == RASCS_Connected || dwError != 0) //连接结束
	{
		Clean();
	}
	
	if (rasconnstat == RASCS_Disconnected || dwError != 0)
	{
		hRasConn = NULL;
	}
}

void TianyiDial::Clean()
{
	if (m_service_thread == NULL)return;

	//终止补丁线程
	m_tydial.Cancel();

	m_service_thread->join();

	delete m_service_thread;
	m_service_thread = NULL;
}

void DialThread(TianyiDial* dial)
{
	USES_CONVERSION;

	dial->m_tydial.PatchAuth();
}

BOOL TianyiDial::SetDialMode(int mode)
{
	switch (mode){
	case 0:
		m_dial_mode = &TianyiDial::DialMode_Emulate;
		break;
	case 1:
		m_dial_mode = &TianyiDial::DialMode_Emulate_new;
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

BOOL TianyiDial::Dial(NetInfo adapter, CString account, CString passwd)
{
	return (this->*m_dial_mode)(adapter, account, passwd);
}

BOOL TianyiDial::Disconnect()
{
	if (hRasConn != NULL){
		RasHangUp(hRasConn);
		RasDialCallback(0, RASCS_Disconnected, 0);
	}

	hRasConn = NULL;

	return 0;
}

BOOL TianyiDial::IsConnected()
{
	return hRasConn != NULL;
}

BOOL TianyiDial::CreatePPPOE(BOOL chap)
{
	//创建拨号连接
	LPRASENTRY lpRasEntry = NULL;
	DWORD cb = sizeof(RASENTRY);
	DWORD dwBufferSize = 0;
	DWORD dwRet = 0;

	// 取得entry的大小,这句也不知道是不是必须的,因为sizeof(RASENTRY)和这里取到的dwBufferSize是一样的,不过还是Get一下安全点
	RasGetEntryProperties(NULL, _T(""), NULL, &dwBufferSize, NULL, NULL);
	if (dwBufferSize == 0)
		return FALSE;

	lpRasEntry = (LPRASENTRY)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBufferSize);
	if (lpRasEntry == NULL)
		return FALSE;

	ZeroMemory(lpRasEntry, sizeof(RASENTRY));
	lpRasEntry->dwSize = dwBufferSize;
	lpRasEntry->dwfOptions = RASEO_PreviewUserPw | RASEO_RemoteDefaultGateway | RASEO_RequirePAP; // RASEO_PreviewUserPw需要显示ui
	if (chap) lpRasEntry->dwfOptions |= RASEO_RequireCHAP;
	lpRasEntry->dwType = RASET_Broadband;
	lpRasEntry->dwEncryptionType = ET_Optional;

	lstrcpy(lpRasEntry->szDeviceType, RASDT_PPPoE);
	lstrcpy(lpRasEntry->szDeviceName, _T(""));
	lpRasEntry->dwfNetProtocols = RASNP_Ip;
	lpRasEntry->dwFramingProtocol = RASFP_Ppp;

	dwRet = RasSetEntryProperties(NULL, _T("Vnet_PPPoE"), lpRasEntry, dwBufferSize, NULL, 0); // 创建连接
	HeapFree(GetProcessHeap(), 0, (LPVOID)lpRasEntry);

	if (dwRet != 0)
		return FALSE;

	return TRUE;
}

BOOL TianyiDial::DialMode_Emulate(NetInfo adapter, CString account, CString passwd)
{
	if (m_service_thread != NULL)return FALSE;

	char dev_name[MAX_ADAPTER_NAME_LENGTH + 30];
	// 用于发送数据包的适配器(名称)
	{
		USES_CONVERSION;
		sprintf_s(dev_name, sizeof(dev_name), "rpcap://\\Device\\NPF_%s", T2A(adapter.sAdapterName));
	}

	m_tydial.Clean();
	Disconnect();

	if (!m_tydial.Init(dev_name))return FALSE;

	if (!CreatePPPOE(TRUE))return FALSE;

	RASDIALPARAMS rdParams;
	DWORD dwRet = 0;
	hRasConn = NULL;

	CString rAccount;
	rAccount.Format(_T("%s%s"), PAP_PREFIX, account);

	ZeroMemory(&rdParams, sizeof(RASDIALPARAMS));
	rdParams.dwSize = sizeof(RASDIALPARAMS);
	lstrcpy(rdParams.szEntryName, _T("Vnet_PPPoE"));
	lstrcpy(rdParams.szUserName, account);
	lstrcpy(rdParams.szPassword, passwd);

	{
		USES_CONVERSION;
		m_tydial.PatchSetup(FALSE, -1, T2A(rAccount), T2A(passwd));
	}


	dwRet = RasDial(NULL, NULL, &rdParams, 0L, (RASDIALFUNC)(this->RasDialCallback), &hRasConn);
	if (dwRet != ERROR_SUCCESS)
	{
		return FALSE;
	}

	//Create thread
	m_service_thread = new std::thread(DialThread, this);

	return TRUE;
}

BOOL TianyiDial::DialMode_Emulate_new(NetInfo adapter, CString account, CString passwd)
{
	if (m_service_thread != NULL)return FALSE;

	char dev_name[MAX_ADAPTER_NAME_LENGTH + 30];
	// 用于发送数据包的适配器(名称)
	{
		USES_CONVERSION;
		sprintf_s(dev_name, sizeof(dev_name), "rpcap://\\Device\\NPF_%s", T2A(adapter.sAdapterName));
	}

	m_tydial.Clean();
	Disconnect();

	if (!m_tydial.Init(dev_name))return FALSE;

	if (!CreatePPPOE(TRUE))return FALSE;

	RASDIALPARAMS rdParams;
	DWORD dwRet = 0;
	hRasConn = NULL;

	CString rAccount;
	rAccount.Format(_T("%s%s"), CHAP_PREFIX, account);

	ZeroMemory(&rdParams, sizeof(RASDIALPARAMS));
	rdParams.dwSize = sizeof(RASDIALPARAMS);
	lstrcpy(rdParams.szEntryName, _T("Vnet_PPPoE"));
	lstrcpy(rdParams.szUserName, rAccount);
	lstrcpy(rdParams.szPassword, passwd);

	{
		USES_CONVERSION;
		m_tydial.PatchSetup(TRUE, -1, T2A(rAccount), T2A(passwd));
	}

	//Create thread
	m_service_thread = new std::thread(DialThread, this);
	//Sleep(100);

	dwRet = RasDial(NULL, NULL, &rdParams, 0L, (RASDIALFUNC)(this->RasDialCallback), &hRasConn);
	if (dwRet != ERROR_SUCCESS)
	{
		m_tydial.Cancel();
		return FALSE;
	}

	return TRUE;
}

BOOL TianyiDial::DialMode_PAP_Prefix(NetInfo adapter, CString account, CString passwd)
{
	Disconnect();

	if (!CreatePPPOE(FALSE))return FALSE;

	RASDIALPARAMS rdParams;
	DWORD dwRet = 0;
	hRasConn = NULL;
	CString rAccount;

	rAccount.Format(_T("%s%s"), PAP_PREFIX, account);

	ZeroMemory(&rdParams, sizeof(RASDIALPARAMS));
	rdParams.dwSize = sizeof(RASDIALPARAMS);
	lstrcpy(rdParams.szEntryName, _T("Vnet_PPPoE"));
	lstrcpy(rdParams.szUserName, rAccount);
	lstrcpy(rdParams.szPassword, passwd);

	dwRet = RasDial(NULL, NULL, &rdParams, 0L, (RASDIALFUNC)(this->RasDialCallback), &hRasConn);
	if (dwRet != ERROR_SUCCESS)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL TianyiDial::DialMode_CHAP_Postfix(NetInfo adapter, CString account, CString passwd)
{
	Disconnect();

	if (!CreatePPPOE(TRUE))return FALSE;

	RASDIALPARAMS rdParams;
	DWORD dwRet = 0;
	hRasConn = NULL;
	CString rAccount;

	rAccount.Format(_T("%s%s"), account, CHAP_POSTFIX);

	ZeroMemory(&rdParams, sizeof(RASDIALPARAMS));
	rdParams.dwSize = sizeof(RASDIALPARAMS);
	lstrcpy(rdParams.szEntryName, _T("Vnet_PPPoE"));
	lstrcpy(rdParams.szUserName, rAccount);
	lstrcpy(rdParams.szPassword, passwd);

	dwRet = RasDial(NULL, NULL, &rdParams, 0L, (RASDIALFUNC)(this->RasDialCallback), &hRasConn);
	if (dwRet != ERROR_SUCCESS)
	{
		return FALSE;
	}

	return TRUE;
}
