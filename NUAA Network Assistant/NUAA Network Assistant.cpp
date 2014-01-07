// NUAA Network Assistant.cpp : main source file for NUAA Network Assistant.exe
//

#include "stdafx.h"

#include "resource.h"

#include "MainDlg.h"
#include "AdapterSelectDlg.h"

#include "Dependency.hpp"

CAppModule _Module;
NetDetector _NetDetector;
NetInfo _NetInfo;
ArpHacker _ArpHacker;
RouteHacker _RouteHacker;
NAT _NAT;
TianyiDial _TianyiDial;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
//	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = 0;
	// BLOCK: Run application
	{
		CString SemaTag, msg;
		SemaTag.LoadString(IDS_TAG_SEMA);
		HANDLE hSem = CreateSemaphore(NULL, 1, 1, SemaTag);
		if (hSem)
		{
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hSem);
				HWND hWndPrev = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
				while (::IsWindow(hWndPrev))
				{
					if (::GetProp(hWndPrev, SemaTag))
					{
						::ShowWindow(hWndPrev, SW_RESTORE);
						::SetForegroundWindow(hWndPrev);
						return FALSE;
					}
					hWndPrev = ::GetWindow(hWndPrev, GW_HWNDNEXT);
				}
				msg.LoadString(IDS_ERR_SEMANOTFOUND);
				MessageBox(NULL, msg, NULL, MB_ICONSTOP);
			}
		}
		else
		{
			msg.LoadString(IDS_ERR_CREATESEMA);
			MessageBox(NULL, msg, NULL, MB_ICONSTOP);
			return FALSE;
		}

		//加载配置
		{
			TCHAR tp[MAX_PATH] = _T("");
			TCHAR config_path[MAX_PATH];
			config_path[0] = '\x0';
			GetModuleFileName(NULL, tp, MAX_PATH);
			PathRemoveFileSpec(tp);
			PathCombine(config_path, tp, _T("config.ini"));
			Config::SetFile(CString(config_path));
			Config::Load();
		}

		{
			if (Config::cfg_firstRun)
			{
				//检测winpcap
				if (!Dependency::CheckWpcap())
				{
					CString no_wpcap;
					no_wpcap.LoadString(IDS_ERR_NOPCAP);
					int nResponse = MessageBox(NULL, no_wpcap, NULL, MB_ICONQUESTION | MB_YESNO);
					if (nResponse == IDNO)
					{
						CString no_install;
						no_install.LoadString(IDS_NO_INSTALL_WPCAP);
						MessageBox(NULL, no_install, NULL, MB_ICONERROR);
					}
					else{
						if (!Dependency::InstallWpcap() || !Dependency::CheckWpcap())
						{
							CString install_fail;
							install_fail.LoadString(IDS_INSTALL_WPCAP_FAIL);
							MessageBox(NULL, install_fail, NULL, MB_ICONERROR);
						}
					}
				}
			}
		}

		if (!_NetDetector.AutoSetAdapter(Config::cfg_adapter) || !_NetDetector.ObtainInformation(_NetInfo)){
			MessageBox(NULL, _T("无法自动检测内网接口，请手动选择正确的网络适配器。"), NULL, MB_ICONERROR);
			CAdapterSelectDlg dlgAdaptSelect;
			nRet = dlgAdaptSelect.DoModal();
			if (nRet){
				dlgAdaptSelect.GetSelNet(_NetInfo);
			}
			else {
				goto quit;
			}
		}

		Config::cfg_adapter = _NetInfo.sAdapterName;
		Config::Save();

		BOOL show = TRUE;
		if (CString(lpstrCmdLine) == _T("hide"))show = FALSE;

		CMainDlg dlgMain(show);
		nRet = dlgMain.DoModal();

		Config::cfg_firstRun = 0;
		Config::Save();
	}

quit:
	_Module.Term();
	::CoUninitialize();

	return nRet;
}
