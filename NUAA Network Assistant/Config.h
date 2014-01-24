#pragma once

class Config
{
	static CString m_filePath;
public:
	static CString cfg_tAccount;
	static CString cfg_tPasswd;
	static CString cfg_adapter;
	static BOOL cfg_firstRun;
	static BOOL cfg_startOnBoot;
	static INT cfg_tDialMode;

	static void SetFile(CString &path);
	static BOOL Load();
	static BOOL Save();

	static BOOL SetBoot(BOOL boot);
};
