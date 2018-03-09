#pragma once
class PEBoundImport
{
private:
	HINSTANCE peInstance = NULL;

public:
	BYTE * pointer = NULL;

private:
	void initList(HWND hDlg);
	void enumModules(HWND hDlg, WPARAM wParam, LPARAM lParam);
	void insterList(HWND blockHwnd, int number, TCHAR *characterOne, TCHAR *characterTWO, TCHAR *characterThree, TCHAR *characterFour);

public:
	PEBoundImport();
	~PEBoundImport();
	void creadPEBoundImportDialog(HINSTANCE PeInstance, BYTE *fpointer);

	friend INT_PTR CALLBACK peBoundImportDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

