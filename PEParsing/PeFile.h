#pragma once
class PeFile
{
private:
	HINSTANCE PEInstance = NULL;

public:
	PeFile();
	~PeFile();

	void creadPEFileDialog(HINSTANCE PeInstance);
	friend INT_PTR CALLBACK PeFileDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

