#include "stdafx.h"
#include "ToggleSettings.h"


ToggleSettings::ToggleSettings()
{
	capsEnabled = false;
	numEnabled = false;
	scrollEnabled = false;
}

ToggleSettings::~ToggleSettings()
{

}

void ToggleSettings::SaveSettings()
{
	TCHAR configPath[MAX_PATH];
	
	SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, configPath);
	CString configFile = configPath;
	configFile += "\\ToggleColors";
	if (GetFileAttributes(configFile)==INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectory(configFile, NULL);
	}

	configFile += "\\settings.ini";
	CStdioFile settingFile;
	if (GetFileAttributes(configFile) != INVALID_FILE_ATTRIBUTES)
	{
		settingFile.Remove(configFile);
		settingFile.Flush();
	}

	settingFile.Open(configFile, CFile::modeCreate | CFile::modeWrite);

	CString a;
	
	//CAPS LOCK
	a.Format(_T("CAPS=%d\n"), (int)capsEnabled);
	settingFile.WriteString(a);
	
	//SCROLL LOCK
	a.Format(_T("SCROLL=%d\n"), (int) scrollEnabled);
	settingFile.WriteString(a);
	
	//NUM LOCK
	a.Format(_T("NUM=%d\n"), (int) numEnabled);
	settingFile.WriteString(a);

	//Red when ON
	settingFile.WriteString(_T("OnR="));
	a.Format(_T("%d\n"), onR);
	settingFile.WriteString(a);

	//Blue when ON
	settingFile.WriteString(_T("OnB="));
	a.Format(_T("%d\n"), onB);
	settingFile.WriteString(a);

	//Green when ON
	settingFile.WriteString(_T("OnG="));
	a.Format(_T("%d\n"), onG);
	settingFile.WriteString(a);

	//Red when OFF
	settingFile.WriteString(_T("OffR="));
	a.Format(_T("%d\n"), offR);
	settingFile.WriteString(a);

	//Blue when OFF
	settingFile.WriteString(_T("OffB="));
	a.Format(_T("%d\n"), offB);
	settingFile.WriteString(a);

	//Green when OFF
	settingFile.WriteString(_T("OffG="));
	a.Format(_T("%d\n"), offG);
	settingFile.WriteString(a);
	
	settingFile.Close();
}

void ToggleSettings::ReadSettings()
{
	TCHAR configPath[MAX_PATH];

	SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, configPath);
	CString configFile = configPath;
	configFile += "\\ToggleColors";
	if (GetFileAttributes(configFile) == INVALID_FILE_ATTRIBUTES)
	{
		//Folder didn't exist let's create it. 
		SaveSettings();
	}

	configFile += "\\settings.ini";
	CStdioFile settingFile;
	if (GetFileAttributes(configFile) == INVALID_FILE_ATTRIBUTES)
	{
		SaveSettings();
	}
	else
	{
		settingFile.Open(configFile, CFile::modeRead);
	}

	CString a;

	//CAPS=
	settingFile.ReadString(a);
	a = a.Mid(5);
	capsEnabled = (a == "1");

	//SCROLL=
	settingFile.ReadString(a);
	a = a.Mid(7);
	scrollEnabled= (a == "1");

	//NUM=
	settingFile.ReadString(a);
	a = a.Mid(4);
	numEnabled = (a == "1");

	//OnR=
	settingFile.ReadString(a);
	a = a.Mid(4);
	onR = _ttoi(a);

	//OnB=
	settingFile.ReadString(a);
	a = a.Mid(4);
	onB = _ttoi(a);

	//OnG=
	settingFile.ReadString(a);
	a = a.Mid(4);
	onG = _ttoi(a);

	//OffR=
	settingFile.ReadString(a);
	a = a.Mid(5);
	offR = _ttoi(a);

	//OffB=
	settingFile.ReadString(a);
	a = a.Mid(5);
	offB = _ttoi(a);

	//OffG=
	settingFile.ReadString(a);
	a = a.Mid(5);
	offG = _ttoi(a);

	settingFile.Close();

}


