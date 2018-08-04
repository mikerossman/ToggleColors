#pragma once
class ToggleSettings
{
public:
	ToggleSettings();
	~ToggleSettings();

	//Write settings to disk
	void SaveSettings();
	//Read settings from disk
	void ReadSettings();

	//Are we watching CAPS lock?
	bool capsEnabled;
	//Are we watching SCROLL lock?
	bool scrollEnabled;
	//Are we watching NUM lock?
	bool numEnabled;

	//Red component of the color when the key is toggled on
	byte onR;
	//Blue component of the color when the key is toggled on
	byte onB;
	//Green component of the color when the key is toggled on
	byte onG;
	//Red component of the color when the key is toggled off
	byte offR;
	//Blue component of the color when the key is toggled off
	byte offB;
	//Green component of the color when the key is toggled off
	byte offG;
};

