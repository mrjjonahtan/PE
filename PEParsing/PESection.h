#pragma once
class PESection
{
public:
	BYTE * pointer = NULL;
private:
	HINSTANCE peInstance = NULL;
public:
	PESection();
	~PESection();

	void creadPESectionDialog(HINSTANCE PeInstance, BYTE *fpointer);
	void initList(HWND hDlg);

	friend INT_PTR CALLBACK PeSectionDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

