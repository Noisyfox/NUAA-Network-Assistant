#pragma once

class Config
{
	static CString m_filePath;
public:
	static CString cfg_tAccount;
	static CString cfg_tPasswd;
	static BOOL cfg_firstRun;
	static BOOL cfg_startOnBoot;

	static void SetFile(CString &path);
	static BOOL Load();
	static BOOL Save();
};
