#include "..\..\stdafx.h"
#include "..\index\PeFile.h"
#include "..\index\PeOptional.h"
#include "..\index\PeDirectory.h"
#include "..\index\PESection.h"
#include "..\index\PEExport.h"
#include "..\index\PERelocation.h"
#include "..\index\PEImport.h"
#include "..\index\PEBoundImport.h"
#include "..\index\PEEditMessage.h"
#include "..\index\PETools.h"
#include "..\..\Tools\PE\PeToolsClass.h"
//�ļ���ק
#include <shellapi.h> 
#pragma comment(lib, "shell32.lib")  


DWORD fileSize = 0;

//�򿪴���
void selectFile(HWND hDlg) {
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	TCHAR strFilename[MAX_PATH] = { 0 };//���ڽ����ļ���  
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hDlg;

	ofn.lpstrFile = strFilename;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(strFilename);
	ofn.lpstrFilter = TEXT("*.exe\0\0");
	ofn.nFilterIndex = 1;

	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = TEXT("��ѡ���ִ���ļ�");
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

//��pe�ص�
INT_PTR CALLBACK DlgProcPEFile(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
		if (staticDlg == NULL) {
			staticDlg = GetDlgItem(hDlg, IDC_TEXT_PE);
		}
		if (editDlg == NULL) {
			editDlg = GetDlgItem(hDlg, IDC_EDIT_MESSAGE);
		}

		SendMessage(staticDlg, WM_SETTEXT, NULL, (LPARAM)L"==================================\n��ѡ���ļ���\n==================================");
		SendDlgItemMessage(hDlg, IDC_PE_OPEN_BTN, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(PEInstance, MAKEINTRESOURCE(IDB_BITMAP_OPENFILE)));
		
		ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
		ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);       // 0x0049 == WM_COPYGLOBALDATA

		//::DragAcceptFiles(staticDlg, TRUE);
		break;
	}
	case WM_DROPFILES:
	{
		HDROP hDrop = (HDROP)wParam;
		UINT  nFileCount = ::DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
		if (nFileCount == 1) {
			wchar_t strFileName[MAX_PATH] = { 0 };
			DragQueryFile(hDrop, 0, strFileName, MAX_PATH);
			wchar_t *scexe = wcsstr(strFileName, L".exe");
			wchar_t *scdll = wcsstr(strFileName, L".dll");
			if (scexe != NULL || scdll != NULL) {
				PEfun(strFileName);
			}
		}
		DragFinish(hDrop);
		break;
	}
	case WM_PAINT:
	{
		//
		HWND edtHwnd = GetDlgItem(hDlg, IDC_EDIT_MESSAGE);

		HFONT hFont = CreateFont(14, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Hebrew");

		SendMessage(edtHwnd, WM_SETFONT, (WPARAM)hFont, 1);

	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case MENU_PE_OPEN_FILE:
		{
			selectFile(hDlg);
			break;
		}
		case IDC_PE_OPEN_BTN:
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
			if (PEInstance != NULL)
			{
				pei = new PEImport();
				if (pei != NULL)
				{
					pei->creadPEImportDialog(PEInstance, pointer);
					delete(pei);
					pei = NULL;
				}

			}
			break;
		}
		case IDC_PE_BI:
		{
			if (PEInstance != NULL)
			{
				pebi = new PEBoundImport();
				if (pebi != NULL)
				{
					pebi->creadPEBoundImportDialog(PEInstance, pointer);
					delete(pebi);
					pebi = NULL;
				}

			}
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
			ShowWindow(editDlg, SW_HIDE);
			ShowWindow(staticDlg, SW_RESTORE);
			break;
		}
		case IDC_PE_STRUCT:
		{
			ShowWindow(editDlg, SW_RESTORE);
			ShowWindow(staticDlg, SW_HIDE);
			if (pointer != NULL) {
				peem = new PEEditMessage();
				if (peem != NULL) {
					peem->setMessageText(hDlg, pointer, fileSize);
					delete(peem);
					peem = NULL;
				}
			}
			break;
		}

		}
		break;
	}
	case WM_CLOSE:
	{
		if (pointer != NULL) {
			/*�ر�ָ��*/
			free(pointer);
			pointer = NULL;
		}
		editDlg = NULL;
		staticDlg = NULL;
		EndDialog(hDlg, 0);
		break;
	}

	}

	return false;
}
/*pe*/
void PEfun(wchar_t *path) {
	FILE *filePointer = NULL;
	//DWORD fileSize = 0;

	fopen_s(&filePointer, ConvertLPWSTRToLPSTR(path), "rb");
	if (filePointer == NULL) {
		return;
	}
	if (fseek(filePointer, 0L, SEEK_END)) {
		return;
	}
	fileSize = ftell(filePointer);
	if (fileSize == 0) {
		return;
	}
	pointer = (BYTE*)malloc(sizeof(BYTE)*fileSize);
	if (pointer == NULL) {
		return;
	}
	memset(pointer, 0L, sizeof(BYTE)*fileSize);
	fseek(filePointer, 0L, 0L);
	if (fread(pointer, sizeof(BYTE)*fileSize, 1, filePointer) <= 0) {
		return;
	}
	/*�ر�filepoint*/
	fclose(filePointer);
	filePointer = NULL;
	PeToolsClass petc;

	if (petc.getDWValue(pointer, 2) == 0x5a4d) {
		/*����ָ��*/
		wchar_t *filesize = TEXT("�ļ���С	��");
		wchar_t *filehead = TEXT("�ļ�ͷ���	��");
		wchar_t *platform = _TEXT("����ƽ̨	��");
		wchar_t *enter = TEXT("\n\0");
		wchar_t showMessage[0x400] = TEXT("============================================\n�ļ�·��	��");
		wchar_t *splitLine = TEXT("\n============================================");
		wchar_t *ispe = L"�Ƿ���PE�ļ�	����";
		wcsncat_s(showMessage, path, wcslen(path));
		wcsncat_s(showMessage, enter, wcslen(enter));
		wcsncat_s(showMessage, ispe, wcslen(ispe));
		wcsncat_s(showMessage, enter, wcslen(enter));
		wcsncat_s(showMessage, filesize, wcslen(filesize));
		wchar_t sd[10] = { 0 };
		_itow_s(fileSize, sd, 10, 10);
		wcsncat_s(showMessage, sd, wcslen(sd));
		wcsncat_s(showMessage, enter, wcslen(enter));
		wcsncat_s(showMessage, filehead, wcslen(filehead));
		wchar_t head[3] = { wchar_t(*pointer),wchar_t(*(pointer + 1)),0 };
		wcsncat_s(showMessage, head, wcslen(head));
		wcsncat_s(showMessage, enter, wcslen(enter));
		wcsncat_s(showMessage, platform, wcslen(platform));
		DWORD platformValue = petc.getApplicationSize(pointer);
		if (platformValue == 0x014C)
		{
			wchar_t *wint = L"Win32λ";
			wcsncat_s(showMessage, wint, wcslen(wint));
		}
		else if (platformValue == 0x8664)
		{
			wchar_t *wint = L"Win64λ";
			wcsncat_s(showMessage, wint, wcslen(wint));
		}
		wcsncat_s(showMessage, splitLine, wcslen(splitLine));
		SendMessage(staticDlg, WM_SETTEXT, NULL, (WPARAM)showMessage);
	}
	else
	{
		wchar_t *message = L"============================================\n��PE�ļ��������BUG��ӭ�ύ�����䣺1640408540@qq.com\n============================================";
		SendMessage(staticDlg, WM_SETTEXT, NULL, (WPARAM)message);
		if (pointer != NULL) {
			free(pointer);
			pointer = NULL;
		}
	}
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
	wchar_t *DOSPoint = NULL;
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

			DOSPoint = (wchar_t*)malloc(sizeof(wchar_t)*DOS_POINT);
			if (DOSPoint == NULL)
			{
				return false;
			}
			memset(DOSPoint, 0, sizeof(wchar_t)*DOS_POINT);
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

/*�ͷ�ָ������*/
void freeSpace(wchar_t *point[]) {
	for (int i = 0; i < DOS_POINT; i++) {
		if (point[i] != NULL) {
			free(point[i]);
		}
	}
}