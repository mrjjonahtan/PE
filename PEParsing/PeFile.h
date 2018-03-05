#pragma once
class PeFile
{
private:
	HINSTANCE peInstance = NULL;
	HWND editHwnd[10] = {NULL};
	WORD hwnds[10] = { IDC_PE_FILE_MACHINE ,IDC_FE_FILE_NUMBEROFSECTIONS ,IDC_FE_FILE_TIMEDATESTAMP ,IDC_FE_FILE_POINTERTOSYMBOLTABLE ,IDC_PE_FILE_NUMBEROFSYMBOLS ,IDC_PE_FILE_SIZEOFOPTIONALHEADER ,IDC_PE_FILE_CHARACTERISTICS ,IDC_PE_FILE_BTN_SAVE ,IDC_PE_FILE_REFRESH ,IDC_PE_FILE_CANCEL};
public:
	BYTE * pointer = NULL;
public:
	PeFile();
	~PeFile();

	void creadPEFileDialog(HINSTANCE PeInstance ,BYTE *fpointer);



	friend INT_PTR CALLBACK PeFileDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

