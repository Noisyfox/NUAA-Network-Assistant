#include "stdafx.h"
#include "Config.h"

void Config::SetFile(CString &path)
{
	m_filePath = path;
}

BOOL Config::Load()
{
	cfg_firstRun = GetPrivateProfileInt(_T("Common"), _T("firstrun"), 1, m_filePath);
	GetPrivateProfileString(_T("tydial"), _T("account"), _T(""), cfg_tAccount.GetBuffer(MAX_PATH), MAX_PATH, m_filePath);
	GetPrivateProfileString(_T("tydial"), _T("passwd"), _T(""), cfg_tPasswd.GetBuffer(MAX_PATH), MAX_PATH, m_filePath);

	return TRUE;
}

BOOL Config::Save()
{
	CString _tmp;
	_tmp.Format(_T("%d"), cfg_firstRun);
	WritePrivateProfileString(_T("Common"), _T("firstrun"), _tmp, m_filePath);
	WritePrivateProfileString(_T("tydial"), _T("account"), cfg_tAccount, m_filePath);
	WritePrivateProfileString(_T("tydial"), _T("passwd"), cfg_tPasswd, m_filePath);

	return TRUE;
}