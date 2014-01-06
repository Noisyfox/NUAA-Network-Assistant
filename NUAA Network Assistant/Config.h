#pragma once

class Config
{
	CString m_filePath;
public:
	CString cfg_tAccount;
	CString cfg_tPasswd;
	BOOL cfg_firstRun;

	void SetFile(CString &path);
	BOOL Load();
	BOOL Save();
};
