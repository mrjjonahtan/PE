#include "stdafx.h"
#include "PeOptional.h"


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