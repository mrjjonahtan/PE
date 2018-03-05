#include "stdafx.h"
#include "PeOptional.h"
#include "PeToolsClass.h"


PeOptional::PeOptional()
{
}


PeOptional::~PeOptional()
{
}

void PeOptional::creadPEOptionalDialog(HINSTANCE PeInstance, BYTE *fpointer)
{
	pointer = fpointer;
	peInstance = PeInstance;
	if (peInstance != NULL)
	{
		DialogBox(peInstance, MAKEINTRESOURCE(IDD_DIALOG_PE_OPTIONAL), NULL, (DLGPROC)PeOptionalDialog);
	}
}

INT_PTR CALLBACK PeOptionalDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	extern PeOptional *peo;
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
		if (peo->editHwnd[0] == NULL)
		{
			for (int i = 0; i < 30; i++)
			{
				peo->editHwnd[i] = GetDlgItem(hDlg, peo->edit[i]);
			}
		}
		if (peo->pointer == NULL)
		{
			for (int i = 0; i < 30; i++)
			{
				SetWindowText(peo->editHwnd[i], L"00000000");
			}
		}
		else
		{
			PeToolsClass petc;
			DWORD optionLocat = petc.getDWValue((peo->pointer + 60), 4) + 24;
			TCHAR *optioBUffer = NULL;
			optioBUffer = (TCHAR*)malloc(sizeof(TCHAR) * 0x200);
			if (optioBUffer == NULL)
			{
				return false;
			}
			memset(optioBUffer, 0, sizeof(TCHAR) * 0x200);

			petc.getValue((peo->pointer + optionLocat), 2, optioBUffer);

			petc.getValue((peo->pointer + optionLocat + 2), 1, optioBUffer + 5);
			petc.getValue((peo->pointer + optionLocat + 3), 1, optioBUffer + 8);

			petc.getValue((peo->pointer + optionLocat + 4), 4, optioBUffer + 11);
			petc.getValue((peo->pointer + optionLocat + 8), 4, optioBUffer + 20);
			petc.getValue((peo->pointer + optionLocat + 12), 4, optioBUffer + 29);
			petc.getValue((peo->pointer + optionLocat + 16), 4, optioBUffer + 38);
			petc.getValue((peo->pointer + optionLocat + 20), 4, optioBUffer + 47);
			petc.getValue((peo->pointer + optionLocat + 24), 4, optioBUffer + 56);
			petc.getValue((peo->pointer + optionLocat + 28), 4, optioBUffer + 65);
			petc.getValue((peo->pointer + optionLocat + 32), 4, optioBUffer + 74);
			petc.getValue((peo->pointer + optionLocat + 36), 4, optioBUffer + 83);

			petc.getValue((peo->pointer + optionLocat + 40), 2, optioBUffer + 92);
			petc.getValue((peo->pointer + optionLocat + 42), 2, optioBUffer + 97);
			petc.getValue((peo->pointer + optionLocat + 44), 2, optioBUffer + 102);
			petc.getValue((peo->pointer + optionLocat + 46), 2, optioBUffer + 107);
			petc.getValue((peo->pointer + optionLocat + 48), 2, optioBUffer + 112);
			petc.getValue((peo->pointer + optionLocat + 50), 2, optioBUffer + 117);

			petc.getValue((peo->pointer + optionLocat + 52), 4, optioBUffer + 122);
			petc.getValue((peo->pointer + optionLocat + 56), 4, optioBUffer + 131);
			petc.getValue((peo->pointer + optionLocat + 60), 4, optioBUffer + 140);
			petc.getValue((peo->pointer + optionLocat + 64), 4, optioBUffer + 149);

			petc.getValue((peo->pointer + optionLocat + 68), 2, optioBUffer + 158);
			petc.getValue((peo->pointer + optionLocat + 70), 2, optioBUffer + 163);

			petc.getValue((peo->pointer + optionLocat + 72), 4, optioBUffer + 168);
			petc.getValue((peo->pointer + optionLocat + 76), 4, optioBUffer + 177);
			petc.getValue((peo->pointer + optionLocat + 80), 4, optioBUffer + 186);
			petc.getValue((peo->pointer + optionLocat + 84), 4, optioBUffer + 195);
			petc.getValue((peo->pointer + optionLocat + 88), 4, optioBUffer + 204);
			petc.getValue((peo->pointer + optionLocat + 92), 4, optioBUffer + 213);

			SetWindowText(peo->editHwnd[0], optioBUffer);
			SetWindowText(peo->editHwnd[1], optioBUffer + 5);
			SetWindowText(peo->editHwnd[2], optioBUffer + 8);
			SetWindowText(peo->editHwnd[3], optioBUffer + 11);
			SetWindowText(peo->editHwnd[4], optioBUffer + 20);
			SetWindowText(peo->editHwnd[5], optioBUffer + 29);
			SetWindowText(peo->editHwnd[6], optioBUffer + 38);
			SetWindowText(peo->editHwnd[7], optioBUffer + 47);
			SetWindowText(peo->editHwnd[8], optioBUffer + 56);
			SetWindowText(peo->editHwnd[9], optioBUffer + 65);
			SetWindowText(peo->editHwnd[10], optioBUffer + 74);
			SetWindowText(peo->editHwnd[11], optioBUffer + 83);
			SetWindowText(peo->editHwnd[12], optioBUffer + 92);
			SetWindowText(peo->editHwnd[13], optioBUffer + 97);
			SetWindowText(peo->editHwnd[14], optioBUffer + 102);
			SetWindowText(peo->editHwnd[15], optioBUffer + 107);
			SetWindowText(peo->editHwnd[16], optioBUffer + 112);
			SetWindowText(peo->editHwnd[17], optioBUffer + 117);
			SetWindowText(peo->editHwnd[18], optioBUffer + 122);
			SetWindowText(peo->editHwnd[19], optioBUffer + 131);
			SetWindowText(peo->editHwnd[20], optioBUffer + 140);
			SetWindowText(peo->editHwnd[21], optioBUffer + 149);
			SetWindowText(peo->editHwnd[22], optioBUffer + 158);
			SetWindowText(peo->editHwnd[23], optioBUffer + 163);
			SetWindowText(peo->editHwnd[24], optioBUffer + 168);
			SetWindowText(peo->editHwnd[25], optioBUffer + 177);
			SetWindowText(peo->editHwnd[26], optioBUffer + 186);
			SetWindowText(peo->editHwnd[27], optioBUffer + 195);
			SetWindowText(peo->editHwnd[28], optioBUffer + 204);
			SetWindowText(peo->editHwnd[29], optioBUffer + 213);
			if (optioBUffer != NULL)
			{
				free(optioBUffer);
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
		case IDC_BTN_PE_OPTIONAL_ERFRESH:
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