#include "stdafx.h"
#include "PEEncryptedshell.h"
#include "PeToolsClass.h"
#include "fileMallocPointer.h"


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
			peencryptedshell->selectFile(hDlg, 1);
			break;
		}
		case IDC_BUTTON_SENCED_SHELL:
			peencryptedshell->selectFile(hDlg, 2);
			break;
		case IDC_BUTTON_ENCRYPTION:
		{
			peencryptedshell->addData(hDlg);
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
void PEEncryptedshell::addData(HWND hDlg)
{
	PeToolsClass petc;
	fileMallocPointer fmp;

	BYTE *shellPointer = NULL;
	BYTE *exePointer = NULL;

	DWORD exeSize = 0;
	DWORD shellSize = 0;
	DWORD lfanew = 0;
	DWORD sizeOfImage = 0;
	DWORD numberOfSections = 0;
	DWORD sizeOfOptionalHeader = 0;

	wchar_t w_filePath[0x150] = { 0 };
	wchar_t w_shellFielPath[0x150] = { 0 };
	char c_filePath[0x150] = { 0 };
	char c_shellFielPath[0x150] = { 0 };

	struct mSection {
		union
		{
			DWORD PhysicalAddress;
			DWORD virtualSize;
		} Misc;
		DWORD virtualAddress;
		DWORD sizeOfRawData;
		DWORD pointertorawdata;
	};
	mSection section[10] = { 0 };

	HWND filePath = GetDlgItem(hDlg, IDC_EDIT_FILE_PATH);
	HWND shellFielPath = GetDlgItem(hDlg, IDC_EDIT_SHELL_FILE_PATH);

	SendMessage(filePath, WM_GETTEXT, 0x150, (LPARAM)w_filePath);
	SendMessage(shellFielPath, WM_GETTEXT, 0x150, (LPARAM)w_shellFielPath);

	if (wcslen(w_filePath) <= 0 && wcslen(w_shellFielPath) <= 0) {
		return;
	}

	sprintf_s(c_filePath, "%ws", w_filePath);
	sprintf_s(c_shellFielPath, "%ws", w_shellFielPath);

	//exe
	exeSize = fmp.filemalloc(c_filePath, &exePointer, 0);

	//shell
	shellSize = fmp.filemalloc(c_shellFielPath, &shellPointer, exeSize);

	lfanew = petc.getPELocation(shellPointer);
	numberOfSections = petc.getSectionNumber(shellPointer);
	sizeOfOptionalHeader = petc.getOptionSizeValue(shellPointer);
	sizeOfImage = petc.getDWValue((shellPointer + lfanew + 24 + 56), 4);

	for (int i = 0; i < numberOfSections; i++)
	{
		DWORD locat = i * 40;
		section[i].Misc.virtualSize = petc.getDWValue((shellPointer + lfanew + sizeOfOptionalHeader + 24 + 8 + locat), 4);
		section[i].virtualAddress = petc.getDWValue((shellPointer + lfanew + sizeOfOptionalHeader + 24 + 12 + locat), 4);
		section[i].sizeOfRawData = petc.getDWValue((shellPointer + lfanew + sizeOfOptionalHeader + 24 + 16 + locat), 4);
		section[i].pointertorawdata = petc.getDWValue((shellPointer + lfanew + sizeOfOptionalHeader + 24 + 20 + locat), 4);
	}

	memcpy((shellPointer + (section[numberOfSections - 1].pointertorawdata + section[numberOfSections - 1].sizeOfRawData)), exePointer, exeSize);

	//更改
	petc.putData((shellPointer + lfanew + 4 + 2), numberOfSections + 1, 2);
	petc.putData((shellPointer + lfanew + 24 + 56), sizeOfImage + exeSize, 4);

	petc.putData((shellPointer + lfanew + sizeOfOptionalHeader + 24 + numberOfSections * 40 ), 0x6A726D2E, 4);
	petc.putData((shellPointer + lfanew + sizeOfOptionalHeader + 24 + numberOfSections  * 40 + 8), exeSize, 4);
	petc.putData((shellPointer + lfanew + sizeOfOptionalHeader + 24 + numberOfSections * 40 + 12), (section[numberOfSections - 1].virtualAddress + petc.getAlignData(section[numberOfSections - 1].Misc.virtualSize, 0x1000)), 4);
	petc.putData((shellPointer + lfanew + sizeOfOptionalHeader + 24 + numberOfSections * 40 + 16), (petc.getAlignData(exeSize, 0x200)), 4);
	petc.putData((shellPointer + lfanew + sizeOfOptionalHeader + 24 + numberOfSections * 40 + 20), (section[numberOfSections - 1].pointertorawdata+ section[numberOfSections - 1].sizeOfRawData), 4);
	petc.putData((shellPointer + lfanew + sizeOfOptionalHeader + 24 + numberOfSections * 40 + 36), 0xE00000E0, 4);


	if (exePointer != NULL) {
		free(exePointer);
		exePointer = NULL;
	}

	//获得当前目录
	wchar_t w_patch[MAX_PATH] = { 0 };
	char patch[MAX_PATH] = { 0 };

	GetModuleFileName(NULL, w_patch, MAX_PATH);
	(_tcsrchr(w_patch, _T('\\')))[1] = 0;

	sprintf_s(patch, "%wsshell.exe", w_patch);

	//
	FILE *fp = NULL;
	int ercod = fopen_s(&fp, patch, "wb");
	if (ercod != 0x00) {
		MessageBox(NULL, L"文件创建失败\n当前路径可能被安全工具限制", L"提示", MB_OK);
	}

	if (fp == NULL)
	{
		return;
	}

	if (fwrite(shellPointer, shellSize, 1, fp) != 1)
	{
		fclose(fp);
		return;
	}

	fclose(fp);

	if (shellPointer != NULL) {
		free(shellPointer);
		shellPointer = NULL;
	}
	MessageBox(NULL, L"Shell程序已生成\n在当前目录。", L"提示", MB_OK);
}

