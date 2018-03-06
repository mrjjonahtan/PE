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
	void insertList(HWND hDlg, int number, TCHAR *name, TCHAR *memory, TCHAR *memorySize, TCHAR *file, TCHAR *fileSize, TCHAR *flag);

	friend INT_PTR CALLBACK PeSectionDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

