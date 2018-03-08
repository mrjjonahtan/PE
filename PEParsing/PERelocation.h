#pragma once
class PERelocation
{
private:
	HINSTANCE peInstance = NULL;
public:
	BYTE * pointer = NULL;
private:
	void initList(HWND hDlg);
	void insterList(HWND hDlg,int number,TCHAR *numberTC, TCHAR *characterOne, TCHAR *characterTWO);
public:
	PERelocation();
	~PERelocation();
	void creadPERelocationDialog(HINSTANCE PeInstance, BYTE *fpointer);

	friend INT_PTR CALLBACK peRelocationDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

