#include "stdafx.h"
#include "PEEditMessage.h"
#include "PeToolsClass.h"

//thread user
HWND	hdlg = NULL;
BYTE*	point = NULL;
DWORD	sizefoe = 0;

PEEditMessage::PEEditMessage()
{

}


PEEditMessage::~PEEditMessage()
{

}


DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	if (sizefoe == 0 || point == NULL || hdlg == NULL) {
		return 1;
	}

	//
	HWND edtHwnd = GetDlgItem(hdlg, IDC_EDIT_MESSAGE);

	SendMessageA(edtHwnd, EM_REPLACESEL, true, (long)"(PE结构)   00  01  02  03  04  05  06  07  08  09  0a  0b  0c  0d  0e  0f\r\n————————————————————————————————————————");

	char strBuffer[0x20] = { 0 };
	
	for (DWORD i = 0; i < 0xf0; i++)//sizefoe
	{

		memset(strBuffer, 0, 0x20);
		if ((i% 16))
		{
			sprintf_s(strBuffer,  "  %02X", (DWORD)*(point + i));
		}
		else
		{
			sprintf_s(strBuffer, " \r\n%08xh: %02X", i,(DWORD)*(point + i));
			//sprintf_s(strBuffer, " %02X", (DWORD)*(point + i));
			
		}
		

		/* 以下两条语句为在edit中追加字符串 */
		//SendMessageA(edtHwnd, EM_SETSEL, -2, -1);
		SendMessageA(edtHwnd, EM_REPLACESEL, true, (long)strBuffer);

		/* 设置滚轮到末尾，这样就可以看到最新信息 */
		//SendMessageA(edtHwnd, WM_VSCROLL, SB_BOTTOM, 0);//SB_BOTTOM
	}

	int df = 0;
	return 0;
}

void PEEditMessage::setMessageText(HWND hDlg, BYTE *pointer, DWORD fileSize) {
	if (fileSize == 0 || hDlg == NULL || pointer == NULL) {
		return;
	}

	hdlg = hDlg;
	point = pointer;
	sizefoe = fileSize;

	//
	CreateThread(NULL, 0, &ThreadProc, 0, 0, 0);

}
