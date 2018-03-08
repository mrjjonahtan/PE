#pragma once
class PERelocation
{
private:
	HINSTANCE peInstance = NULL;
public:
	BYTE * pointer = NULL;
private:
	void initList(HWND hDlg);
	void enumModules(HWND hDlg, WPARAM wParam, LPARAM lParam);
	void disBlocksMembers(HWND membersHwnd,DWORD offset);
	void insterList(HWND blockHwnd,int number,TCHAR *numberTC, TCHAR *characterOne, TCHAR *characterTWO, TCHAR *characterThree);
public:
	PERelocation();
	~PERelocation();
	void creadPERelocationDialog(HINSTANCE PeInstance, BYTE *fpointer);

	friend INT_PTR CALLBACK peRelocationDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

