#pragma once
class PEExport
{
public:
	BYTE * pointer = NULL;
private:
	HINSTANCE peInstance = NULL;
	int editID[10] = { IDC_EDIT_PE_EXPORT_OFFSETTOEXPORTTABLE,
		IDC_EDIT_PE_EXPORT_CHARACTERISTICS,
		IDC_EDIT_PE_EXPORT_BASE,
		IDC_EDIT_PE_EXPORT_NAME,
		IDC_EDIT_PE_EXPORT_NAMESTRING,

		IDC_EDIT_PE_EXPORT_NUMBEROFFUNCTIONS,
		IDC_EDIT_PE_EXPORT_NUMBEROFNAMES,
		IDC_EDIT_PE_EXPORT_ADDRESSOFFUNCTIONS,
		IDC_EDIT_PE_EXPORT_ADDRESSOFNAMES,
		IDC_EDIT_PE_EXPORT_ADDRESSOFNAMEORDINALS };
	HWND editHwnd[10] = {NULL};
public:
	PEExport();
	~PEExport();

	void creadPEExportDialog(HINSTANCE PeInstance, BYTE *fpointer);
	void initList(HWND hDlg);
	void insertList(HWND hDlg, int number, TCHAR *ordinals, TCHAR *rva, TCHAR *offset, TCHAR *name);
	friend INT_PTR CALLBACK peExportDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

