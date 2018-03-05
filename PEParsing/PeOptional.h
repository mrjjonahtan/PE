#pragma once
class PeOptional
{
private:
	HINSTANCE peInstance = NULL;
public:
	BYTE * pointer = NULL;
public:
	PeOptional();
	~PeOptional();
	void creadPEOptionalDialog(HINSTANCE PeInstance, BYTE *fpointer);

	friend INT_PTR CALLBACK PeOptionalDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

