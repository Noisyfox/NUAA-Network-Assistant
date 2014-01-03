#include "stdafx.h"
#include "resource.h"

#include "AdapterSelectDlg.h"


LRESULT CAdapterSelectDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	CButton okButton = GetDlgItem(IDOK);
	okButton.EnableWindow(FALSE);

	//获取所有适配器
	PIP_ADAPTER_INFO pAdapter = NULL;
	if (!_NetDetector.ListAllAdapter(pAdapter, FALSE)){
		MessageBox(_T("获取适配器列表失败！"), NULL, MB_ICONERROR);
		return FALSE;
	}

	CComboBox combo = GetDlgItem(IDC_ADAPTERLIST);
	BOOL hasAdapter = pAdapter != NULL;

	while (pAdapter){
		combo.AddString(CString(pAdapter->Description));
		pAdapter = pAdapter->Next;
	}

	if (hasAdapter){
		if (_NetDetector.ObtainInformation(selectedNet)){
			combo.SelectString(0, selectedNet.sAdapterDesc);
			ShowNetInfo(&selectedNet);

			okButton.EnableWindow(TRUE);
		}
		else {
			combo.SetCurSel(0);
			BOOL FAKE;
			OnComboAdapterSelChange(NULL, NULL, combo, FAKE);
		}
	}
	else {
		ShowNetInfo(NULL);
	}

	return TRUE;
}

LRESULT CAdapterSelectDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(TRUE);
	return 0;
}

LRESULT CAdapterSelectDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(FALSE);
	return 0;
}

LRESULT CAdapterSelectDlg::OnComboAdapterSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/){
	CComboBox combo = (CComboBox)hWndCtl;
	CButton okButton = GetDlgItem(IDOK);
	okButton.EnableWindow(FALSE);

	CString strCBText;
	combo.GetWindowText(strCBText);

	//获取所有适配器
	PIP_ADAPTER_INFO pAdapter = NULL;
	if (!_NetDetector.ListAllAdapter(pAdapter, FALSE)){
		ShowNetInfo(NULL);

		return FALSE;
	}

	while (pAdapter){
		if (CString(pAdapter->Description) == strCBText){
			break;
		}
		pAdapter = pAdapter->Next;
	}

	if (!pAdapter){
		ShowNetInfo(NULL);
		return FALSE;
	}

	if (!_NetDetector.SetAdapter(pAdapter) || !_NetDetector.ObtainInformation(selectedNet)){
		ShowNetInfo(NULL);
		return FALSE;
	}

	ShowNetInfo(&selectedNet);

	okButton.EnableWindow(TRUE);

	return TRUE;
}

void CAdapterSelectDlg::ShowNetInfo(NetInfo * info){
	if (info){
		SetDlgItemText(IDC_GROUP_NETINFO, info->sAdapterDesc);
		SetDlgItemText(IDC_NETINFO_LOCMAC, info->sLocalMac);
		SetDlgItemText(IDC_NETINFO_LOCIP, info->sLocalIp);
		SetDlgItemText(IDC_NETINFO_LOCMASK, info->sLocalMask);
		SetDlgItemText(IDC_NETINFO_GATEWAYIP, info->sGatewayIp);
		SetDlgItemText(IDC_NETINFO_GATEWAYMAC, info->sGatewayMac);
	}
	else {
		SetDlgItemText(IDC_GROUP_NETINFO, _T("获取适配器信息失败！"));
		SetDlgItemText(IDC_NETINFO_LOCMAC, _T("00-00-00-00-00-00"));
		SetDlgItemText(IDC_NETINFO_LOCIP, _T("0.0.0.0"));
		SetDlgItemText(IDC_NETINFO_LOCMASK, _T("0.0.0.0"));
		SetDlgItemText(IDC_NETINFO_GATEWAYIP, _T("0.0.0.0"));
		SetDlgItemText(IDC_NETINFO_GATEWAYMAC, _T("00-00-00-00-00-00"));
	}
}

void CAdapterSelectDlg::GetSelNet(NetInfo & info){
	info = selectedNet;
}
