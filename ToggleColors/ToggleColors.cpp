
// MFCApplication1.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ToggleColors.h"
#include "ToggleSettingsDlg.h"
#include "CUESDK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1App

BEGIN_MESSAGE_MAP(CToggleColors, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// The one and only CMFCApplication1App object
CToggleColors theApp;

CWnd* mainWindow; 

// CMFCApplication1App construction

CToggleColors::CToggleColors()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CToggleColors::~CToggleColors()
{
	//Stop receiving keyboard messages from the OS
	UnhookWindowsHookEx(hookHandle);
	//Get rid of the system tray icon
	//Shell_NotifyIcon(NIM_DELETE, &notifyData);
}

// CMFCApplication1App initialization
BOOL CToggleColors::InitInstance()
{

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	//CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	
	//Handshake with the Corsair keyboard
	CorsairProtocolDetails d;
	d = CorsairPerformProtocolHandshake();

	//Tell the OS to start sending us keyboard events
	hookHandle = ::SetWindowsHookEx(WH_KEYBOARD_LL, this->KeyboardHook, nullptr, 0);

	//Modeless dialog that allows configuration of colors and which keys to watch. 
	//Dialog is set invisible. 
	m_pMainWnd = new CToggleSettingsDlg;

		
#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	//TRUE: Start message pump
	return TRUE;
}

//This function processes the incoming keyboard messages from the OS. Emphasis is on fast return
//for keys we don't care about. We also don't want to do mem allocations unless we know we need them. 
//Remember, this is going to fire for every key press!
LRESULT CALLBACK CToggleColors::KeyboardHook(int code, WPARAM wParam, LPARAM lParam)
{
	//Only react to key down events
	if (wParam != WM_KEYDOWN) { return ::CallNextHookEx(nullptr, code, wParam, lParam); }

	LPKBDLLHOOKSTRUCT hookStruct;
	hookStruct = (LPKBDLLHOOKSTRUCT)lParam;
	DWORD keyPressed = hookStruct->vkCode;

	//Only process if the key pressed was caps lock, scroll lock, or num lock and we're configured to watch the key pressed
	if (!((keyPressed == VK_CAPITAL && theApp.appSettings.capsEnabled) ||
		(keyPressed == VK_NUMLOCK && theApp.appSettings.numEnabled) ||
		(keyPressed == VK_SCROLL && theApp.appSettings.scrollEnabled)))
	{
		return ::CallNextHookEx(nullptr, code, wParam, lParam);
	}

	//We know we need to do something, get the state of the toggle key pressed
	CorsairLedColor myLedColors[1];
	short keyState;
	bool invert = false;    // Invert the behaviour (use the opposite color)
	switch (keyPressed)
	{
	case VK_CAPITAL: keyState = GetKeyState(VK_CAPITAL);
		myLedColors[0].ledId = CorsairLedId::CLK_CapsLock;
		break;
	case VK_NUMLOCK: keyState = GetKeyState(VK_NUMLOCK);
		myLedColors[0].ledId = CorsairLedId::CLK_NumLock;
		if (theApp.appSettings.numInverted) invert = true;
		break;
	case VK_SCROLL: keyState = GetKeyState(VK_SCROLL);
		myLedColors[0].ledId = CorsairLedId::CLK_ScrollLock;
		break;
	}

	//Figure out what colors we're going to set based on the key state. 
	//Least significant bit set to 1 if the key is toggled on. Note this is executed before the OS or hardware changes the 
	//state. So we're using reverse logic and acting on what the state will become, NOT what it presently is. 
	if ((keyState & 0x0001) ^ invert)
	{
		//Key is transitioning to OFF
		myLedColors[0].b = theApp.appSettings.offB;
		myLedColors[0].g = theApp.appSettings.offG;
		myLedColors[0].r = theApp.appSettings.offR;
	}
	else
	{
		//Key is transitioning to ON
		myLedColors[0].b = theApp.appSettings.onB;
		myLedColors[0].g = theApp.appSettings.onG;
		myLedColors[0].r = theApp.appSettings.onR;
	}

	//Make the color change
	bool setColorResult = CorsairSetLedsColors(1, myLedColors);

	//Fix if ToggleColors was started before iCUE finished loading.
	//If fail, redo the protocol handshake and then retry the set. 
	if (!setColorResult)
	{
		CorsairProtocolDetails dtls = CorsairPerformProtocolHandshake();
		if (dtls.serverProtocolVersion != 0) { CorsairSetLedsColors(1, myLedColors); }
	}

	//Pass the message on to anyone else that has a hook. 
	return ::CallNextHookEx(nullptr, code, wParam, lParam);
}






