#include "stdafx.h"
#include "PEEditMessage.h"


PEEditMessage::PEEditMessage()
{
}


PEEditMessage::~PEEditMessage()
{
	if (edtHwnd != NULL) {
		edtHwnd = NULL;
	}
}

void PEEditMessage::setMessageText(HWND hDlg, BYTE *pointer) {
	edtHwnd = GetDlgItem(hDlg, IDC_EDIT_MESSAGE);
	OutputDebugString(L"sss");
}
