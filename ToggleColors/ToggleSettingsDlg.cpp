// MFCApplication1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ToggleColors.h"
#include "ToggleSettingsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_USER_SYSTRAY WM_USER+1

// CMFCApplication1Dlg dialog
CToggleSettingsDlg::CToggleSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Create(IDD_MFCAPPLICATION1_DIALOG, pParent);

}

void CToggleSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CToggleSettingsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COLOR_OFF, &CToggleSettingsDlg::OnBnClickedColorOff)
	ON_BN_CLICKED(IDC_COLOR_ON, &CToggleSettingsDlg::OnBnClickedColorOn)
	ON_BN_CLICKED(IDC_ENABLE_NUMLOCK, &CToggleSettingsDlg::OnBnClickedEnableNumlock)
	ON_BN_CLICKED(IDC_ENABLE_SCROLLOCK, &CToggleSettingsDlg::OnBnClickedEnableScrollock)
	ON_BN_CLICKED(IDC_ENABLE_CAPSLOCK, &CToggleSettingsDlg::OnBnClickedEnableCapslock)
	ON_BN_CLICKED(IDOK, &CToggleSettingsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CToggleSettingsDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_USER_SYSTRAY, OnSysTray)
	ON_BN_CLICKED(IDC_INVERT_NUMLOCK, &CToggleSettingsDlg::OnBnClickedInvertNumlock)
END_MESSAGE_MAP()


// CMFCApplication1Dlg message handlers

BOOL CToggleSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->ShowWindow(SW_HIDE);
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//Read settings from application config file
	theApp.appSettings.ReadSettings();

	//Colors for when the toggle key is on or off
	CMFCColorButton* colorOff = (CMFCColorButton*)GetDlgItem(IDC_COLOR_OFF);
	CMFCColorButton* colorOn = (CMFCColorButton*)GetDlgItem(IDC_COLOR_ON);
	colorOff->SetColor(RGB(theApp.appSettings.offR, theApp.appSettings.offG, theApp.appSettings.offB));
	colorOn->SetColor(RGB(theApp.appSettings.onR, theApp.appSettings.onG, theApp.appSettings.onB));
	
	//Which keys are we going to watch?
	//CAPS
	CButton* chk = (CButton*)GetDlgItem(IDC_ENABLE_CAPSLOCK);
	chk->SetCheck(theApp.appSettings.capsEnabled);
	//NUM 
	chk = (CButton*)GetDlgItem(IDC_ENABLE_NUMLOCK);
	chk->SetCheck(theApp.appSettings.numEnabled);
	//NUM_INV
	chk = (CButton*)GetDlgItem(IDC_INVERT_NUMLOCK);
	chk->SetCheck(theApp.appSettings.numInverted);
	//SCROLL
	chk = (CButton*)GetDlgItem(IDC_ENABLE_SCROLLOCK);
	chk->SetCheck(theApp.appSettings.scrollEnabled);
	
	//Create system tray icon. Wait for WM_USER+1 events. 
	notifyData.cbSize = sizeof(NOTIFYICONDATA);
	notifyData.hWnd = m_hWnd;
	notifyData.uID = 100;
	notifyData.uVersion = NOTIFYICON_VERSION;
	notifyData.uCallbackMessage = WM_USER + 1;
	wcscpy_s(notifyData.szTip, _T("CAPS/SCROLL/NUM Lock Color Toggle"));
	notifyData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	notifyData.hIcon = LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	Shell_NotifyIcon(NIM_ADD, &notifyData);
	this->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CToggleSettingsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CToggleSettingsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CToggleSettingsDlg::OnBnClickedColorOff()
{
	//Capture changes to the color desired when a key is toggled OFF
	CMFCColorButton* colorOff = (CMFCColorButton*)GetDlgItem(IDC_COLOR_OFF);
	theApp.appSettings.offR = GetRValue(colorOff->GetColor());
	theApp.appSettings.offB = GetBValue(colorOff->GetColor());
	theApp.appSettings.offG = GetGValue(colorOff->GetColor());
	theApp.appSettings.SaveSettings();
}

void CToggleSettingsDlg::OnBnClickedColorOn()
{
	//Cap[ture changes to the color desired when a key is toggled ON
	CMFCColorButton* colorOn = (CMFCColorButton*)GetDlgItem(IDC_COLOR_ON);
	theApp.appSettings.onR = GetRValue(colorOn->GetColor());
	theApp.appSettings.onB = GetBValue(colorOn->GetColor());
	theApp.appSettings.onG = GetGValue(colorOn->GetColor());
	theApp.appSettings.SaveSettings();
}

void CToggleSettingsDlg::OnBnClickedEnableNumlock()
{
	//Capture whether we're watching the NUM lock key
	CButton* chk = (CButton*)GetDlgItem(IDC_ENABLE_NUMLOCK);
	theApp.appSettings.numEnabled = chk->GetCheck()==1;
	theApp.appSettings.SaveSettings();
}

void CToggleSettingsDlg::OnBnClickedInvertNumlock() {
	//Capture whether we're inverting the NUM lock key
	CButton* chk = (CButton*)GetDlgItem(IDC_INVERT_NUMLOCK);
	theApp.appSettings.numInverted = chk->GetCheck();
	theApp.appSettings.SaveSettings();
}

void CToggleSettingsDlg::OnBnClickedEnableScrollock()
{
	//Capture whether we're watching the SCROLL lock key. 
	CButton* chk = (CButton*)GetDlgItem(IDC_ENABLE_SCROLLOCK);
	theApp.appSettings.scrollEnabled = chk->GetCheck()==1;
	theApp.appSettings.SaveSettings();
}

void CToggleSettingsDlg::OnBnClickedEnableCapslock()
{
	//Capture whether we're watching the CAPS lock key. 
	CButton* chk = (CButton*)GetDlgItem(IDC_ENABLE_CAPSLOCK);
	theApp.appSettings.capsEnabled = chk->GetCheck()==1;
	theApp.appSettings.SaveSettings();
}

void CToggleSettingsDlg::OnBnClickedOk()
{
	//Write current settings to disk
	theApp.appSettings.SaveSettings();
	this->ShowWindow(SW_HIDE);
}

void CToggleSettingsDlg::OnClose()
{
	CDialogEx::OnClose();
}


void CToggleSettingsDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	Shell_NotifyIcon(NIM_DELETE, &notifyData);
	DestroyWindow();
}

//Event handler for user interaction with the system tray icon. 
afx_msg LRESULT CToggleSettingsDlg::OnSysTray(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case WM_RBUTTONDOWN:
	{
		CMenu menu, *pSubMenu;
		menu.LoadMenu(IDR_MENU1);
		pSubMenu = menu.GetSubMenu(0);
		CPoint p;
		GetCursorPos(&p);
		SetForegroundWindow();
		BOOL cmd = pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, p.x, p.y, this);
		pSubMenu->DestroyMenu();
		menu.DestroyMenu();
		
		switch (cmd)
		{
		case ID_CONFIGURETOGGLECOLORS_EXIT:
			this->OnBnClickedCancel();
			break;
		case ID_CONFIGURETOGGLECOLORS_CONFIG:
			this->ShowWindow(SW_NORMAL);
			break;
		}
		break;
	}
	case WM_LBUTTONDBLCLK:
	{
		this->ShowWindow(SW_NORMAL);
		break;
	}
	}
	return NULL;
}

void CToggleSettingsDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}
