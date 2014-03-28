// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "Dependency.hpp"
#include "MainDlg.h"
#include "AdapterSelectDlg.h"
#include "Util.h"

CMainDlg::CMainDlg(BOOL show)
{
	m_appName.LoadString(IDR_MAINFRAME);
	m_strDialConnect.LoadString(IDS_BTN_DIAL_CONNECT);
	m_strDialDisconnect.LoadString(IDS_BTN_DIAL_DISCONNECT);

	hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	m_msgTaskbarRestart = RegisterWindowMessage(_T("TaskbarCreated"));

	m_hideOnStart = !show;
}

BOOL CMainDlg::CreateNotificationIcon()
{
	//Create notification icon
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID = IDR_MAINFRAME;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nid.uCallbackMessage = WM_NOTIFICATION_ICON;             // �Զ������Ϣ����
	m_nid.hIcon = hIconSmall;
	wcscpy_s(m_nid.szTip, m_appName);
	return Shell_NotifyIcon(NIM_ADD, &m_nid);                // �����������ͼ��
}

BOOL CMainDlg::DeleteNotificationIcon()
{
	// ��������ɾ��ͼ��
	return Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

BOOL CMainDlg::EnsureWpcap()
{
	//���winpcap
	if (!Dependency::CheckWpcap())
	{
		CString no_wpcap;
		no_wpcap.LoadString(IDS_ERR_NOPCAP_MUST);
		int nResponse = MessageBox(no_wpcap, NULL, MB_ICONQUESTION | MB_YESNO);
		if (nResponse == IDNO)
		{
			CString no_install;
			no_install.LoadString(IDS_NO_INSTALL_WPCAP_MUST);
			MessageBox(no_install, NULL, MB_ICONERROR);

			return FALSE;
		}
		else{
			if (!Dependency::InstallWpcap() || !Dependency::CheckWpcap())
			{
				CString install_fail;
				install_fail.LoadString(IDS_INSTALL_WPCAP_FAIL_MUST);
				MessageBox(install_fail, NULL, MB_ICONERROR);

				return FALSE;
			}
		}
	}

	return TRUE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CString SemaTag;
	SemaTag.LoadString(IDS_TAG_SEMA);
	::SetProp(m_hWnd, SemaTag, (HANDLE)1);

	// center the dialog on the screen
	CenterWindow();

	// set icons
	SetIcon(hIcon, TRUE);
	SetIcon(hIconSmall, FALSE);

	m_btnEnableLan = GetDlgItem(IDC_CHECK_ENABLELAN);
	m_btnEnableRedi = GetDlgItem(IDC_CHECK_ENABLEREDI);
	m_btnEnableCampus = GetDlgItem(IDC_CHECK_ENABLECAMPUS);
	m_btnDial = GetDlgItem(IDC_DIAL);
	m_btnBoot = GetDlgItem(IDC_CHECK_STARTONBOOT);

	m_edtDialAccount = GetDlgItem(IDC_DIAL_ACCOUNT);
	m_edtDialPasswd = GetDlgItem(IDC_DIAL_PASSWD);

	m_comboDialMode = GetDlgItem(IDC_DIALMODE);

	CreateNotificationIcon();

	ShowNetInfo(&_NetInfo);

	if (_NetInfo.hasGateway){
		m_btnEnableLan.SetCheck(TRUE);
		m_btnEnableCampus.SetCheck(TRUE);
		SendMessage(WM_COMMAND, IDC_CHECK_ENABLELAN, NULL);
		SendMessage(WM_COMMAND, IDC_CHECK_ENABLECAMPUS, NULL);
	}

	ToggleLanFunc(_NetInfo.hasGateway);

	_TianyiDial.RegisterMessage(this->m_hWnd);

	//��������
	m_edtDialAccount.SetWindowText(Config::cfg_tAccount);
	m_edtDialPasswd.SetWindowText(Config::cfg_tPasswd);
	m_btnBoot.SetCheck(Config::cfg_startOnBoot);

	CString dialMode;
	dialMode.LoadString(IDS_DIALMODE);
	int dialModeCount = _TianyiDial.GetDialModeCount();
	for (int i = 1; i <= dialModeCount; i++){
		CString _t;
		_t.Format(_T("%s%d"), dialMode, i);
		m_comboDialMode.AddString(_t);
	}

	if (!_TianyiDial.SetDialMode(Config::cfg_tDialMode)){
		Config::cfg_tDialMode = 0;
	}
	m_comboDialMode.SetCurSel(Config::cfg_tDialMode);

	return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString exit_cfm;
	exit_cfm.LoadString(IDS_EXIT_CONFIRM);
	int nResponse = MessageBox(exit_cfm, m_appName, MB_ICONQUESTION | MB_YESNO);
	if (nResponse == IDNO)
	{
		return 0;
	}

	if (_ArpHacker.IsArpHacked())_ArpHacker.FlushArp();
	if (_RouteHacker.IsRouteHacked())_RouteHacker.FlushRoute();
	_NAT.Uninstall();

	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnChangeAdapter(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/){
	CAdapterSelectDlg dlgAdaptSelect;
	int nRet = dlgAdaptSelect.DoModal();
	if (nRet){
		dlgAdaptSelect.GetSelNet(_NetInfo);
		ShowNetInfo(&_NetInfo);

		Config::cfg_adapter = _NetInfo.sAdapterName;
		Config::Save();

		m_btnEnableLan.SetCheck(FALSE);
		m_btnEnableRedi.SetCheck(FALSE);
		m_btnEnableCampus.SetCheck(FALSE);
		if (_ArpHacker.IsArpHacked())_ArpHacker.FlushArp();
		if (_RouteHacker.IsRouteHacked())_RouteHacker.FlushRoute();

		_NAT.Uninstall();

		_TianyiDial.RegisterMessage(this->m_hWnd);

		ToggleLanFunc(_NetInfo.hasGateway);
	}

	return 0;
}

void CMainDlg::ToggleLanFunc(BOOL hasLan){
	m_btnEnableLan.EnableWindow(hasLan);
	m_btnEnableRedi.EnableWindow(hasLan);
	m_btnEnableCampus.EnableWindow(hasLan);
}

LRESULT CMainDlg::OnDial(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

	if (!EnsureWpcap())return 0;

	CString account, passwd;
	m_edtDialAccount.GetWindowText(account);
	m_edtDialPasswd.GetWindowText(passwd);

	Config::cfg_tAccount = account;
	Config::cfg_tPasswd = passwd;
	Config::Save();

	//SetDlgItemText(IDC_DIAL_STATUS, account);

	if (_TianyiDial.IsConnected()){
		m_btnDial.EnableWindow(FALSE);
		_TianyiDial.Disconnect();
	}
	else {
		if (_TianyiDial.Dial(_NetInfo, account, passwd))
		{
			m_comboDialMode.EnableWindow(FALSE);
			m_edtDialAccount.SetReadOnly(TRUE);
			m_edtDialPasswd.SetReadOnly(TRUE);
		}
	}
	
	return 0;
}

LRESULT CMainDlg::OnEnableLanClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL check = m_btnEnableLan.GetCheck();

	if (check){
		if (!_ArpHacker.Init(_NetInfo) || !_ArpHacker.HackArp()){
			m_btnEnableLan.SetCheck(FALSE);
		}
	}
	else {
		if (_ArpHacker.IsArpHacked())_ArpHacker.FlushArp();
	}

	return 0;
}

LRESULT CMainDlg::OnEnableRediClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL check = m_btnEnableRedi.GetCheck();

	if (check){
		if (!EnsureWpcap()){
			m_btnEnableRedi.SetCheck(FALSE);
			return 0;
		}

		int nRet = _NAT.Setup(_NetInfo);
		if (nRet != NO_ERROR && nRet != ERROR_SERVICE_ALREADY_RUNNING)
		{
			m_btnEnableRedi.SetCheck(FALSE);
		}
	}
	else {
		_NAT.Uninstall();
	}

	return 0;
}

LRESULT CMainDlg::OnEnableCampusClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL check = m_btnEnableCampus.GetCheck();

	if (check){
		if (!_RouteHacker.Init(_NetInfo) || !_RouteHacker.HackRoute(FALSE)){
			m_btnEnableCampus.SetCheck(FALSE);
		}
	}
	else {
		if (_RouteHacker.IsRouteHacked())_RouteHacker.FlushRoute();
	}

	return 0;
}

LRESULT CMainDlg::OnStartOnBootClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL check = m_btnBoot.GetCheck();

	Config::cfg_startOnBoot = check;
	Config::Save();

	Config::SetBoot(check);

	return 0;
}

LRESULT CMainDlg::OnComboDialModeSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
	CComboBox combo = (CComboBox)hWndCtl;

	int sel = combo.GetCurSel();

	if (!_TianyiDial.SetDialMode(sel)){
		_TianyiDial.SetDialMode(Config::cfg_tDialMode);
		combo.SetCurSel(Config::cfg_tDialMode);
	}
	else {
		Config::cfg_tDialMode = sel;
		Config::Save();
	}

	return 0;
}

LRESULT CMainDlg::OnNotiToggleEnableLan(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_btnEnableLan.SetCheck(!m_btnEnableLan.GetCheck());
	SendMessage(WM_COMMAND, IDC_CHECK_ENABLELAN, NULL);
	return 0;
}

LRESULT CMainDlg::OnNotiToggleEnableRedi(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_btnEnableRedi.SetCheck(!m_btnEnableRedi.GetCheck());
	SendMessage(WM_COMMAND, IDC_CHECK_ENABLEREDI, NULL);
	return 0;
}

LRESULT CMainDlg::OnNotiToggleCampusNet(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_btnEnableCampus.SetCheck(!m_btnEnableCampus.GetCheck());
	SendMessage(WM_COMMAND, IDC_CHECK_ENABLECAMPUS, NULL);
	return 0;
}

LRESULT CMainDlg::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (m_hideOnStart)
	{
		m_hideOnStart = FALSE;
		ShowWindow(SW_HIDE);//��������ʱ���ش���
	}

	if (wParam == SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE); // ����С��ʱ������������              
	}

	return 0;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DeleteNotificationIcon();

	CString SemaTag;
	SemaTag.LoadString(IDS_TAG_SEMA);
	::RemoveProp(m_hWnd, SemaTag);

	return 0;
}

LRESULT CMainDlg::OnNoti(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if (wParam != IDR_MAINFRAME)
		return 1;

	switch (lParam)
	{
	case WM_RBUTTONUP:
	{
						 POINT point;
						 ::GetCursorPos(&point);
						 CMenu menu;
						 menu.LoadMenu(IDR_NOTIMENU);
						 CMenu cMenu = menu.GetSubMenu(0);
						 cMenu.CheckMenuItem(ID_NOTI_ENLAN, m_btnEnableLan.GetCheck() ? MF_CHECKED : MF_UNCHECKED);
						 cMenu.CheckMenuItem(ID_NOTI_ENREDI, m_btnEnableRedi.GetCheck() ? MF_CHECKED : MF_UNCHECKED);
						 cMenu.CheckMenuItem(ID_NOTI_ENCAMPUS, m_btnEnableCampus.GetCheck() ? MF_CHECKED : MF_UNCHECKED);
						 if (!_NetInfo.hasGateway){
							 cMenu.EnableMenuItem(ID_NOTI_ENLAN, MF_GRAYED);
							 cMenu.EnableMenuItem(ID_NOTI_ENREDI, MF_GRAYED);
							 cMenu.EnableMenuItem(ID_NOTI_ENCAMPUS, MF_GRAYED);
						 }
						 cMenu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, m_hWnd);
						 HMENU hmenu = cMenu.Detach();
                         menu.DestroyMenu();
						 cMenu.DestroyMenu();
	}
		break;
	case WM_LBUTTONDBLCLK:
	{
							 if (IsWindowVisible()){
								 ShowWindow(SW_HIDE);
							 }
							 else {
								 ShowWindow(SW_SHOWNORMAL);
								 SetForegroundWindow(SetActiveWindow());
							 }
	}
		break;
	}
	return 0;
}

//�������������ؽ�ͼ��
LRESULT CMainDlg::OnTaskbarRestart(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DeleteNotificationIcon();
	CreateNotificationIcon();

	return 0;
}

LRESULT CMainDlg::OnDial(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	RASCONNSTATE rasconnstat = (RASCONNSTATE)wParam;
	DWORD dwError = (DWORD)lParam;

	_TianyiDial.OnCallback(rasconnstat, dwError);

	CString str;

	switch (rasconnstat)
	{
	case RASCS_OpenPort: str = _T("���ڴ򿪶˿�..."); break;
	case RASCS_PortOpened: str = _T("�˿��Ѿ���"); break;
	case RASCS_ConnectDevice: str = _T("���������豸..."); break;
	case RASCS_DeviceConnected: str = _T("�豸�Ѿ�����"); break;
	case RASCS_AllDevicesConnected: str = _T("�����豸�Ѿ�����"); break;
	case RASCS_Authenticate: str = _T("������֤..."); break;
	case RASCS_AuthNotify: str = _T("��ʼ��֤..."); break;
	case RASCS_AuthRetry: str = _T("������֤..."); break;
	case RASCS_AuthCallback: str = _T("��֤�ز�..."); break;
	case RASCS_AuthChangePassword: str = _T("�����޸�����..."); break;
	case RASCS_AuthProject: str = _T("AuthProject"); break;
	case RASCS_AuthLinkSpeed: str = _T("���ڼ�����������..."); break;
	case RASCS_AuthAck: str = _T("��֤ͨ��..."); break;
	case RASCS_ReAuthenticate: str = _T("����������֤..."); break;
	case RASCS_Authenticated: str = _T("��֤���..."); break;
	case RASCS_PrepareForCallback: str = _T("׼���ز�..."); break;
	case RASCS_WaitForModemReset: str = _T("�ȴ�����..."); break;
	case RASCS_WaitForCallback: str = _T("�ȴ��ز�..."); break;
	case RASCS_Projected: str = _T("Projected"); break;
	case RASCS_SubEntryConnected: str = _T("����ڵ�������"); break;
	case RASCS_SubEntryDisconnected: str = _T("����ڵ��ѶϿ�"); break;
		//case RASCS_ApplySettings: str = _T("����Ӧ������..."); break;
	case RASCS_Interactive: str = _T("���ڽ���..."); break;
	case RASCS_RetryAuthentication: str = _T("��������..."); break;
	case RASCS_CallbackSetByCaller: str = _T("CallbackSetByCaller"); break;
	case RASCS_PasswordExpired: str = _T("�����ѹ���"); break;
	case RASCS_InvokeEapUI: str = _T("InvokeEapUI"); break;
	case RASCS_Connected: str = _T("�Ѿ�����"); break;
	case RASCS_Disconnected: str = _T("�Ѿ��Ͽ�"); break;
	default: str.Format(_T("״̬�룺%d"), rasconnstat); break;
	}

	if (dwError != 0)
	{
		str = DialGetError(dwError);
	}

	SetDlgItemText(IDC_DIAL_STATUS, str);

	m_btnDial.SetWindowText(m_strDialDisconnect);

	if (dwError != 0 || rasconnstat == RASCS_Disconnected){
		m_comboDialMode.EnableWindow(TRUE);
		m_btnDial.EnableWindow(TRUE);
		m_edtDialAccount.SetReadOnly(FALSE);
		m_edtDialPasswd.SetReadOnly(FALSE);
		m_btnDial.SetWindowText(m_strDialConnect);
	}
	else if (rasconnstat == RASCS_Connected){
		m_btnDial.EnableWindow(TRUE);
	}

	return 0;
}

void CMainDlg::ShowNetInfo(NetInfo * info){
	if (info){
		SetDlgItemText(IDC_NETINFO_ADAPNAME, info->sAdapterDesc);
		SetDlgItemText(IDC_NETINFO_LOCIP, info->sLocalIp);
		SetDlgItemText(IDC_NETINFO_LOCMASK, info->sLocalMask);
		SetDlgItemText(IDC_NETINFO_GATEWAYIP, info->sGatewayIp);
		if (info->hasGateway){
			SetDlgItemText(IDC_NETINFO_GATEWAYMAC, info->sGatewayMac);
		}
		else{
			SetDlgItemText(IDC_NETINFO_GATEWAYMAC, _T("00-00-00-00-00-00"));
		}
	}
	else {
		SetDlgItemText(IDC_NETINFO_ADAPNAME, _T(""));
		SetDlgItemText(IDC_NETINFO_LOCIP, _T("0.0.0.0"));
		SetDlgItemText(IDC_NETINFO_LOCMASK, _T("0.0.0.0"));
		SetDlgItemText(IDC_NETINFO_GATEWAYIP, _T("0.0.0.0"));
		SetDlgItemText(IDC_NETINFO_GATEWAYMAC, _T("00-00-00-00-00-00"));
	}
}
