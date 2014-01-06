#pragma once

#ifndef _DEPENDENCY_HPP_
#define _DEPENDENCY_HPP_

class Dependency
{
public:
	static CString GetExecPath()
	{
		static CString ExecPath;
		static BOOL _pathGet = FALSE;

		if (!_pathGet)
		{
			TCHAR tp[MAX_PATH] = _T("");
			GetModuleFileName(NULL, tp, MAX_PATH);
			PathRemoveFileSpec(tp);
			ExecPath = CString(tp);

			_pathGet = TRUE;
		}

		return ExecPath;
	}

	static BOOL CheckWpcap()
	{
		HINSTANCE hInst = ::LoadLibrary(_T("wpcap.dll"));
		if (NULL == hInst)
			return FALSE;

		::FreeLibrary(hInst);

		return TRUE;
	}

	static BOOL InstallWpcap()
	{
		TCHAR installer_path[MAX_PATH];
		installer_path[0] = '\x0';
		PathCombine(installer_path, GetExecPath(), _T("WinPcap_4_1_2.exe"));

		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = installer_path;
		ShExecInfo.lpParameters = _T("");
		ShExecInfo.lpDirectory = GetExecPath();
		ShExecInfo.nShow = SW_SHOWNORMAL;
		ShExecInfo.hInstApp = NULL;


		ShellExecuteEx(&ShExecInfo);

		if (ShExecInfo.hProcess)
		{
			::WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
			::CloseHandle(ShExecInfo.hProcess);
			return TRUE;
		}

		return FALSE;
	}
};

#endif // _DEPENDENCY_HPP_
