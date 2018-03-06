#include "stdafx.h"
#include "PeDirectory.h"
#include "PeToolsClass.h"


PeDirectory::PeDirectory()
{
}


PeDirectory::~PeDirectory()
{
}

void PeDirectory::creadPEDirectoryDialog(HINSTANCE PeInstance, BYTE *fpointer)
{
	pointer = fpointer;
	peInstance = PeInstance;
	if (peInstance != NULL)
	{
		DialogBox(peInstance, MAKEINTRESOURCE(IDD_DIALOG_PE_DIRECTORY), NULL, (DLGPROC)PeDirectoryDialog);
	}
}

INT_PTR CALLBACK PeDirectoryDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	extern PeDirectory *ped;
	if (ped->editHwnd[0] == NULL)
	{
		for (int i = 0; i < 32; i++)
		{
			ped->editHwnd[i] = GetDlgItem(hDlg, ped->editId[i]);

		}
	}
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
		if (ped->pointer == NULL)
		{
			for (int i = 0; i < 32; i++)
			{
				SetWindowText(ped->editHwnd[i], L"00000000");
			}
		}
		else
		{
			PeToolsClass petc;
			TCHAR *temporaryBUffer = NULL;
			temporaryBUffer = (TCHAR*)malloc(sizeof(TCHAR) * 0x200);
			if (temporaryBUffer == NULL)
			{
				return false;
			}
			memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x200);

			DWORD directoryLocat = petc.getDWValue((ped->pointer + 60), 4) + 24 + 96;

			for (int i = 0; i < 32; i++)
			{
				petc.getValue((ped->pointer + directoryLocat + (i * 4)), 4, (temporaryBUffer+(i*9)));
				SetWindowText(ped->editHwnd[i], temporaryBUffer + (i * 9));
			}
			if (temporaryBUffer != NULL)
			{
				free(temporaryBUffer);
				temporaryBUffer = NULL;
			}
		}
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BTN_PE_OPTIONAL_SAVE:
		{
			break;
		}
		case IDC_BTN_PE_OPTIONAL_REFRESH:
		{
			break;
		}
		case IDC_BTN_PE_OPTIONAL_CANCEL:
		{
			EndDialog(hDlg, 0);
			break;
		}
		}
		break;
	}
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