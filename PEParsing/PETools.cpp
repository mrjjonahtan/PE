#include "stdafx.h"
#include "PeFile.h"
#include "PeOptional.h"
#include "PeDirectory.h"
#include "PESection.h"
#include "PEExport.h"
#include "PERelocation.h"
#include "PETools.h"
#include "PeToolsClass.h"


//打开窗口
void selectFile(HWND hDlg) {
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	TCHAR strFilename[MAX_PATH] = { 0 };//用于接收文件名  
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hDlg;

	ofn.lpstrFile = strFilename;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(strFilename);
	ofn.lpstrFilter = TEXT("*.exe\0\0");
	ofn.nFilterIndex = 1;

	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = TEXT("请选择可执行文件");
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	if (GetOpenFileName(&ofn)) {
		if (staticDlg != NULL) {
			PEfun(strFilename);
		}
	}
}

void CreatePEDialog(HINSTANCE thisInstance, HWND hDlg) {
	PEInstance = thisInstance;
	if (PEInstance != NULL) {
		DialogBox(PEInstance, MAKEINTRESOURCE(IDD_DIALOG_PETOOLS), hDlg, (DLGPROC)DlgProcPEFile);
	}
}

//打开pe回掉
INT_PTR CALLBACK DlgProcPEFile(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{

		if (staticDlg == NULL) {
			staticDlg = GetDlgItem(hDlg, IDC_TEXT_PE);
		}
		SendMessage(staticDlg, WM_SETTEXT, NULL, (LPARAM)L"请选择文件。");


		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case MENU_PE_OPEN_FILE:
		{
			selectFile(hDlg);
			break;
		}
		case IDC_PE_B:
		{
			if (PEInstance != NULL)
			{
				per = new PERelocation();
				if (per != NULL)
				{
					per->creadPERelocationDialog(PEInstance, pointer);
					delete(per);
					per = NULL;
				}

			}
			break;
		}
		case IDC_PE_S:
		{
			if (PEInstance != NULL)
			{
				pes = new PESection();
				if (pes != NULL)
				{
					pes->creadPESectionDialog(PEInstance, pointer);
					delete(pes);
					pes = NULL;
				}

			}
			break;
		}
		case IDC_PE_L:
		{
			if (PEInstance != NULL)
			{
				ped = new PeDirectory();
				if (ped != NULL)
				{
					ped->creadPEDirectoryDialog(PEInstance, pointer);
					delete(ped);
					ped = NULL;
				}

			}
			break;
		}
		case IDC_PE_I:
		{

			break;
		}
		case IDC_PE_O:
		{
			if (PEInstance != NULL)
			{
				peo = new PeOptional();
				if (peo != NULL)
				{
					peo->creadPEOptionalDialog(PEInstance, pointer);
					delete(peo);
					peo = NULL;
				}

			}
			break;
		}
		case IDC_PE_D:
		{
			if (PEInstance != NULL) {
				DialogBox(PEInstance, MAKEINTRESOURCE(IDD_PE_DIALOG_DOS), hDlg, (DLGPROC)DlgProcPEDOS);
			}
			break;
		}
		case IDC_PE_F:
		{
			if (PEInstance != NULL) {
				pef = new PeFile();
				if (pef != NULL)
				{
					pef->creadPEFileDialog(PEInstance, pointer);
					delete(pef);
					pef = NULL;
				}

			}
			break;
		}
		case IDC_PE_E:
		{
			if (PEInstance != NULL) {
				pee = new PEExport();
				if (pee != NULL)
				{
					pee->creadPEExportDialog(PEInstance, pointer);
					delete(pee);
					pee = NULL;
				}

			}
			break;
		}
		case IDC_PE_FILE_MESSAGE:
		{

			break;
		}
		case IDC_PE_STRUCT:
		{

			break;
		}

		}
		break;
	}
	case WM_CLOSE:
	{
		if (pointer != NULL) {
			/*关闭指针*/
			free(pointer);
			pointer = NULL;
		}
		EndDialog(hDlg, 0);
		break;
	}

	}

	return false;
}
/*pe*/
void PEfun(TCHAR *path) {
	FILE *filePointer = NULL;
	DWORD size = 0;

	fopen_s(&filePointer, ConvertLPWSTRToLPSTR(path), "rb");
	if (filePointer == NULL) {
		return;
	}
	if (fseek(filePointer, 0L, SEEK_END)) {
		return;
	}
	size = ftell(filePointer);
	if (size == 0) {
		return;
	}
	pointer = (BYTE*)malloc(sizeof(BYTE)*size);
	if (pointer == NULL) {
		return;
	}
	memset(pointer, 0L, sizeof(BYTE)*size);
	fseek(filePointer, 0L, 0L);
	if (fread(pointer, sizeof(BYTE)*size, 1, filePointer) <= 0) {
		return;
	}
	/*关闭filepoint*/
	fclose(filePointer);
	filePointer = NULL;

	/*操作指针*/
	TCHAR *filesize = TEXT("文件大小：\0");
	TCHAR *filehead = TEXT("文件头标记：\0");
	TCHAR *enter = TEXT("\n\0");
	TCHAR showMessage[0x400] = TEXT("文件路径：\0");

	wcsncat_s(showMessage, path, wcslen(path));
	wcsncat_s(showMessage, enter, wcslen(enter));
	wcsncat_s(showMessage, filesize, wcslen(filesize));
	TCHAR sd[10] = { 0 };
	_itow_s(size, sd, 10, 10);
	wcsncat_s(showMessage, sd, wcslen(sd));
	wcsncat_s(showMessage, enter, wcslen(enter));
	wcsncat_s(showMessage, filehead, wcslen(filehead));
	TCHAR head[3] = { TCHAR(*pointer),TCHAR(*(pointer + 1)),0 };
	wcsncat_s(showMessage, head, wcslen(head));
	SendMessage(staticDlg, WM_SETTEXT, NULL, (WPARAM)showMessage);
}

/*TCHAR to char*/
char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn)
{
	LPSTR pszOut = NULL;
	if (lpwszStrIn != NULL)
	{
		int nInputStrLen = wcslen(lpwszStrIn);

		// Double NULL Termination  
		int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
		pszOut = new char[nOutputStrLen];

		if (pszOut)
		{
			memset(pszOut, 0x00, nOutputStrLen);
			WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
		}
	}
	return pszOut;
}

/*DOS*/
INT_PTR CALLBACK DlgProcPEDOS(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	int editID[20] = { IDC_EDIT_PE_MAGIC ,IDC_EDIT_PE_CBLP, IDC_EDIT_PE_CP ,IDC_EDIT_PE_CRLC ,IDC_EDIT_PE_CPARHDR,IDC_EDIT_PE_MINALLOC,IDC_EDIT_PE_MAXALLOC,IDC_EDIT_PE_SS,IDC_EDIT_PE_SP,IDC_EDIT_PE_CSUM,IDC_EDIT_PE_IP,IDC_EDIT_PE_CS,IDC_EDIT_PE_LFARLC,IDC_EDIT_PE_OVNO,IDC_EDIT_PE_RES,IDC_EDIT_PE_OEMID,IDC_EDIT_PE_OEMINFO,IDC_EDIT_PE_RES2,IDC_EDIT_PE_LFANEW,0 };
	HWND editHwnd[20] = { 0 };
	TCHAR *DOSPoint = NULL;
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
		if (pointer != NULL) {
			PeToolsClass petc;
			for (int i = 0; i < 20 && editID[i] != 0; i++)
			{
				editHwnd[i] = GetDlgItem(hDlg, editID[i]);
			}

			DOSPoint = (TCHAR*)malloc(sizeof(TCHAR)*DOS_POINT);
			if (DOSPoint == NULL)
			{
				return false;
			}
			memset(DOSPoint, 0, sizeof(TCHAR)*DOS_POINT);
			for (int i = 0; i < 14; i++)
			{
				if (i == 0)
				{
					petc.getValue((pointer), 2, DOSPoint);
					SetWindowText(editHwnd[i], DOSPoint);
				}
				else
				{
					petc.getValue((pointer + (i * 2)), 2, DOSPoint + (i * 4 + i));
					SetWindowText(editHwnd[i], (DOSPoint + (i * 4 + i)));
				}

			}

			petc.getValue((pointer + (14 * 2)), (4 * 2), DOSPoint + 65);
			petc.getValue((pointer + (18 * 2)), 2, DOSPoint + 98);
			petc.getValue((pointer + (19 * 2)), 2, DOSPoint + 107);
			petc.getValue((pointer + (20 * 2)), (10 * 2), DOSPoint + 116);
			petc.getValue((pointer + (30 * 2)), 4, DOSPoint + 196);
			SetWindowText(editHwnd[14], (DOSPoint + 65));
			SetWindowText(editHwnd[15], (DOSPoint + 98));
			SetWindowText(editHwnd[16], (DOSPoint + 107));
			SetWindowText(editHwnd[17], (DOSPoint + 116));
			SetWindowText(editHwnd[18], (DOSPoint + 196));
		}
		else
		{
			for (int i = 0; editID[i] != 0; i++)
			{
				editHwnd[i] = GetDlgItem(hDlg, editID[i]);
				SetWindowText(editHwnd[i], L"00000000");
			}
		}
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_PE_BTN_SUMBIT:
		{
			if (DOSPoint != NULL)
			{
				free(DOSPoint);
			}
			EndDialog(hDlg, 0);
			break;
		}
		}
		break;
	}
	case WM_CLOSE:
	{
		if (DOSPoint != NULL)
		{
			free(DOSPoint);
		}
		EndDialog(hDlg, 0);
		break;
	}
	}
	return false;
}
/*
void getValue(BYTE *pointerValue, int number, TCHAR *tvlue) {
	char *vaby = NULL;
	vaby = (char*)malloc(0x200);
	if (vaby == NULL)
	{
		return;
	}
	for (int i = number - 1, j = 0; i >= 0; i--, j++)
	{
		memset(vaby, 0, 0x200);
		DWORD valueTem = *(pointerValue + i);
		sprintf_s(vaby, 0x200, "%X", valueTem);
		for (int k = 0; *(vaby + k) != 0; k++)
		{
			*(tvlue + j) = *(vaby + k);
			j++;
		}
		j--;
	}
	if (vaby != NULL) {
		free(vaby);
	}
}
*/
/*释放指针数组*/
void freeSpace(TCHAR *point[]) {
	for (int i = 0; i < DOS_POINT; i++) {
		if (point[i] != NULL) {
			free(point[i]);
		}
	}
}
