#include "stdafx.h"
#include "PEEncryptedshell.h"


PEEncryptedshell::PEEncryptedshell()
{
	//IDD_DIALOG_ENCRYPTEDSHELL
}


PEEncryptedshell::~PEEncryptedshell()
{
}

void PEEncryptedshell::createDialog(HINSTANCE PeInstance)
{
	if (PeInstance != NULL) {
		peInstance = PeInstance;
		DialogBox(peInstance, MAKEINTRESOURCE(IDD_DIALOG_ENCRYPTEDSHELL), NULL, (DLGPROC)encryptedShellDialog);
	}
}

INT_PTR CALLBACK encryptedShellDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	extern PEEncryptedshell *peencryptedshell;
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_SENCED:
		{
			peencryptedshell->selectFile(hDlg,1);
			break; 
		}
		case IDC_BUTTON_SENCED_SHELL:
			peencryptedshell->selectFile(hDlg, 2);
			break;
		case IDC_BUTTON_ENCRYPTION:
		{

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

void PEEncryptedshell::selectFile(HWND hDlg, BYTE style) {
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
		wchar_t *scexe = wcsstr(strFilename, L".exe");
		wchar_t *scdll = wcsstr(strFilename, L".dll");
		if (scexe != NULL || scdll != NULL) {
			if (style == 1)
			{
				HWND btn = GetDlgItem(hDlg, IDC_EDIT_FILE_PATH);
				SendMessage(btn, WM_SETTEXT, NULL, (LPARAM)strFilename);
			}
			else if (style == 2)
			{
				HWND btn = GetDlgItem(hDlg, IDC_EDIT_SHELL_FILE_PATH);
				SendMessage(btn, WM_SETTEXT, NULL, (LPARAM)strFilename);
			}
			
		}
	}
}

//加密
void PEEncryptedshell::encryptionAlgorithm()
{
	//value ^ 0x55;
}

//解密
void PEEncryptedshell::decryptionAlgorithm()
{
	//value ^ 0x55;
}

//添加
void PEEncryptedshell::addData()
{

}
