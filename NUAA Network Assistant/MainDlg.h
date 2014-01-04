// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define WM_NOTIFICATION_ICON WM_USER + 101

class CMainDlg : public CDialogImpl<CMainDlg>
{
private:
	NOTIFYICONDATA m_nid;
	UINT m_msgTaskbarRestart;
	HICON hIcon, hIconSmall;
	CString m_appName, m_strDialConnect, m_strDialDisconnect;

	void ShowNetInfo(NetInfo * info);
	BOOL CreateNotificationIcon();
	BOOL DeleteNotificationIcon();

	CButton m_btnEnableLan, m_btnEnableRedi, m_btnEnableCampus, m_btnDial;
	CEdit m_edtDialAccount, m_edtDialPasswd;
public:

	CMainDlg();

	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_NOTIFICATION_ICON, OnNoti)
		MESSAGE_HANDLER(m_msgTaskbarRestart, OnTaskbarRestart)
		MESSAGE_HANDLER(WM_RASDIAL_UPDATE, OnDial)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_CHANGEADAP, OnChangeAdapter)
		COMMAND_ID_HANDLER(IDC_DIAL, OnDial)
		COMMAND_ID_HANDLER(IDC_CHECK_ENABLELAN, OnEnableLanClicked)
		COMMAND_ID_HANDLER(IDC_CHECK_ENABLEREDI, OnEnableRediClicked)
		COMMAND_ID_HANDLER(IDC_CHECK_ENABLECAMPUS, OnEnableCampusClicked)
		COMMAND_ID_HANDLER(ID_NOTI_ENLAN, OnNotiToggleEnableLan)
		COMMAND_ID_HANDLER(ID_NOTI_ENREDI, OnNotiToggleEnableRedi)
		COMMAND_ID_HANDLER(ID_NOTI_ENCAMPUS, OnNotiToggleCampusNet)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNoti(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnTaskbarRestart(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDial(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnChangeAdapter(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDial(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnableLanClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnableRediClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnableCampusClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNotiToggleEnableLan(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNotiToggleEnableRedi(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNotiToggleCampusNet(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
