#include "stdafx.h"
#include "PEEditMessage.h"
#include "PeToolsClass.h"


PEEditMessage::PEEditMessage()
{
	
}


PEEditMessage::~PEEditMessage()
{
	
}

void PEEditMessage::setMessageText(HWND hDlg, BYTE *pointer, DWORD fileSize) {
	if ( fileSize == 0) {
		return;
	}

	//
	HWND edtHwnd = GetDlgItem(hDlg, IDC_EDIT_MESSAGE);

	SendMessageA(edtHwnd, EM_REPLACESEL, true, (long)"PE结构：\r\n--------------------------------------\r\n");


	char strBuffer[10] = { 0 };
	for (int i = 0; i < fileSize; i++)
	{
		memset(strBuffer, 0, 10);
		sprintf_s(strBuffer, ((i+1) % 16 == 0) ? " %02X\r\n" : " %02X", (DWORD)*(pointer + i));

		/* 以下两条语句为在edit中追加字符串 */
		//SendMessageA(edtHwnd, EM_SETSEL, -2, -1);
		SendMessageA(edtHwnd, EM_REPLACESEL, true, (long)strBuffer);

		/* 设置滚轮到末尾，这样就可以看到最新信息 */
		//SendMessageA(edtHwnd, WM_VSCROLL, SB_BOTTOM, 0);
	}

}

