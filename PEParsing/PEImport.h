#pragma once
class PEImport
{
private:
	HINSTANCE peInstance = NULL;
public:
	BYTE * pointer = NULL;
private:
	void initList(HWND hDlg);
	void enumModules(HWND hDlg, WPARAM wParam, LPARAM lParam);
	void disIatIntMembers(HWND membersHwnd, DWORD iatRVA, DWORD intRVA);
	void insterList(HWND blockHwnd, int number, TCHAR *characterOne, TCHAR *characterTWO, TCHAR *characterThree, TCHAR *characterFour);

public:
	PEImport();
	~PEImport();
	void creadPEImportDialog(HINSTANCE PeInstance, BYTE *fpointer);

	friend INT_PTR CALLBACK peImportDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

