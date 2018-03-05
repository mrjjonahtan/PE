#include "stdafx.h"
#include "PeFile.h"


PeFile::PeFile()
{
}


PeFile::~PeFile()
{
}

void PeFile::creadPEFileDialog(HINSTANCE PeInstance)
{
	if (PEInstance == NULL)
	{
		DialogBox(PEInstance, MAKEINTRESOURCE(IDD_PE_DIALOG_FILE), NULL, PeFileDialog);
	}
}

INT_PTR CALLBACK PeFileDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CLOSE:
	{
		EndDialog(hDlg, 0);
		break;
	}
	default:
		break;
	}
	return false;
}