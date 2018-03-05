#include "stdafx.h"
#include "PeFile.h"
#include "PeToolsClass.h"

PeFile::PeFile()
{

}


PeFile::~PeFile()
{
	pointer = NULL;
}

void PeFile::creadPEFileDialog(HINSTANCE PeInstance, BYTE *fpointer)
{
	peInstance = PeInstance;
	pointer = fpointer;
	if (peInstance != NULL)
	{
		DialogBox(peInstance, MAKEINTRESOURCE(IDD_PE_DIALOG_FILE), NULL, (DLGPROC)PeFileDialog);
	}
	
}

INT_PTR CALLBACK PeFileDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	extern PeFile *pef;
	if (pef->editHwnd[0] == NULL) {
		for (int i = 0; i < 10; i++)
		{
			pef->editHwnd[i] = GetDlgItem(hDlg, pef->hwnds[i]);
		}
	}
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
		if (pef->pointer != NULL)
		{
		PeToolsClass petc;
		TCHAR *fileBuffer = NULL;
		fileBuffer = (TCHAR*)malloc(sizeof(TCHAR) * 0x40);
		if (fileBuffer == NULL)
		{
			return false;
		}
		memset(fileBuffer, 0, sizeof(TCHAR) * 0x40);
		DWORD peLocat = petc.getDWValue((pef->pointer + 60), 4);
		petc.getValue((pef->pointer + peLocat + 4), 2, fileBuffer);
		petc.getValue((pef->pointer + peLocat + 4 + 2), 2, fileBuffer + 6);
		petc.getValue((pef->pointer + peLocat + 4 + 4), 4, fileBuffer + 11);
		petc.getValue((pef->pointer + peLocat + 4 + 8), 4, fileBuffer + 21);
		petc.getValue((pef->pointer + peLocat + 4 + 12), 4, fileBuffer + 31);
		petc.getValue((pef->pointer + peLocat + 4 + 16), 2, fileBuffer + 41);
		petc.getValue((pef->pointer + peLocat + 4 + 18), 2, fileBuffer + 47);

		SetWindowText(pef->editHwnd[0], fileBuffer);
		SetWindowText(pef->editHwnd[1], (fileBuffer + 6));
		SetWindowText(pef->editHwnd[2], (fileBuffer + 11));
		SetWindowText(pef->editHwnd[3], (fileBuffer + 21));
		SetWindowText(pef->editHwnd[4], (fileBuffer + 31));
		SetWindowText(pef->editHwnd[5], (fileBuffer + 41));
		SetWindowText(pef->editHwnd[6], (fileBuffer + 47));
		}
		else
		{
			for (int i = 0; i < 7; i++)
			{
				SetWindowText(pef->editHwnd[i], L"00000000");
			}
		}
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_PE_FILE_CANCEL:
		{
			EndDialog(hDlg, 0);
		}
		break;
		case IDC_PE_FILE_REFRESH:
		{

		}
		break;
		case IDC_PE_FILE_BTN_SAVE:
		{

		}
		break;
		}

	}
	break;
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