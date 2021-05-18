
// MFCApplication1Dlg.h : header file
//

#pragma once
#include "afxwin.h"


// CMFCApplication1Dlg dialog
class CToggleSettingsDlg : public CDialogEx
{
// Construction
public:
	CToggleSettingsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedColorOff();
	afx_msg void OnBnClickedColorOn();
	afx_msg void OnBnClickedEnableNumlock();
	afx_msg void OnBnClickedInvertNumlock();
	afx_msg void OnBnClickedEnableScrollock();
	afx_msg void OnBnClickedEnableCapslock();

	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnSysTray(WPARAM, LPARAM);
	
	virtual void PostNcDestroy();

private:
	NOTIFYICONDATA notifyData; 
};
