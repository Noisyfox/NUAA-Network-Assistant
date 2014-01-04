#pragma once

class Config
{
	CString m_filePath;
public:
	CString cfg_tAccount;
	CString cfg_tPasswd;

	void SetFile(CString &path);
	BOOL Load();
	BOOL Save();
};
