#include "stdafx.h"
#include "PEEditMessage.h"
#include "PeToolsClass.h"


PEEditMessage::PEEditMessage()
{
	message = (wchar_t*)malloc(sizeof(wchar_t) * 0x7530);
	temChar = (wchar_t*)malloc(sizeof(wchar_t) * 0x400);
}


PEEditMessage::~PEEditMessage()
{
	if (edtHwnd != NULL) {
		edtHwnd = NULL;
	}
	if (message != NULL) {
		free(message);
		message = NULL;
	}
	if (temChar != NULL) {
		free(temChar);
		temChar = NULL;
	}
}

void PEEditMessage::setMessageText(HWND hDlg, BYTE *pointer) {
	if (message == NULL || temChar == NULL) {
		return;
	}
	memset(message, 0, sizeof(wchar_t) * 0x7530);
	memset(temChar, 0, sizeof(wchar_t) * 0x400);

	
	additionalCharts(L"================================================================================================\r\n");
	edtHwnd = GetDlgItem(hDlg, IDC_EDIT_MESSAGE);
	SendMessage(edtHwnd, WM_SETTEXT, NULL, (WPARAM)message);
} 

void PEEditMessage::additionalCharts(wchar_t *messageT) {
	if (message == NULL || temChar == NULL) {
		return;
	}
	wsprintf(temChar, L"%s", messageT);
	wcsncat_s(message, sizeof(wchar_t) * 0x400, temChar, _TRUNCATE);
	memset(temChar, 0, sizeof(wchar_t) * 0x400);
}
