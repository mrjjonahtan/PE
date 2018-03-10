#pragma once
class PEBoundImport
{
private:
	HINSTANCE peInstance = NULL;
	DWORD boundFOA[100] = {0};
public:
	BYTE * pointer = NULL;

private:
	void initList(HWND hDlg);
	void enumModules(HWND hDlg, WPARAM wParam, LPARAM lParam);
	void insterList(HWND blockHwnd, int number, TCHAR *characterOne, TCHAR *characterTWO, TCHAR *characterThree, TCHAR *characterFour, TCHAR *characterFive);
	void disMembers(HWND membersHwnd, DWORD number);
public:
	PEBoundImport();
	~PEBoundImport();
	void creadPEBoundImportDialog(HINSTANCE PeInstance, BYTE *fpointer);

	friend INT_PTR CALLBACK peBoundImportDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

