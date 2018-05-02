#include "stdafx.h"
#include "InjectionAndUnloading.h"


InjectionAndUnloading::InjectionAndUnloading()
{
	peInstance = NULL;
	hDllMod = NULL;
}


InjectionAndUnloading::~InjectionAndUnloading()
{
}

void InjectionAndUnloading::createDialog(HINSTANCE PeInstance)
{
	if (PeInstance != NULL) {
		peInstance = PeInstance;
		DialogBox(peInstance, MAKEINTRESOURCE(IDD_DIALOG_DLL_INJECTION), NULL, (DLGPROC)injectionDialog);
	}
}

INT_PTR CALLBACK injectionDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	extern InjectionAndUnloading *injectunloading;
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
		if (injectunloading->peInstance != NULL)
		{
			::SendDlgItemMessage(hDlg, IDC_SENECT_PATH, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(injectunloading->peInstance, MAKEINTRESOURCE(IDB_BITMAP_OPENFILE)));
		}
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_SENECT_PATH:
		{
			injectunloading->selectFile(hDlg);
		}
		break;
		case IDC_BUTTON_DLL_INJECTION:
		{
			injectunloading->operating(hDlg, 1);
		}
		break;
		case IDC_BUTTON_UNINSTALL:
		{
			injectunloading->operating(hDlg, 2);
		}
		break;
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

void InjectionAndUnloading::selectFile(HWND hDlg)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	TCHAR strFilename[MAX_PATH] = { 0 };//用于接收文件名  
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hDlg;

	ofn.lpstrFile = strFilename;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(strFilename);
	ofn.lpstrFilter = TEXT("*.dll\0\0");
	ofn.nFilterIndex = 1;

	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = TEXT("选择文件");
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	if (GetOpenFileName(&ofn)) {

		if (wcslen(strFilename))
		{
			HWND pathEdit = ::GetDlgItem(hDlg, IDC_EDIT_DELL_PATH);
			::SendMessage(pathEdit, WM_SETTEXT, NULL, (LPARAM)strFilename);

		}

	}
}

//
int InjectionAndUnloading::remoteThreadInjection(DWORD pid, wchar_t *path)
{
	PVOID  address = 0;
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess == NULL)
	{
		return 0;
	}

	//
	/*wchar_t param[0x50] = { 0 };
	swprintf_s(param, 0x50, L"%s", L"E://DllInjection.dll");*/

	address = ::VirtualAllocEx(hProcess, NULL, 0x60, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (address == NULL)
	{
		VirtualFreeEx(hProcess, NULL, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 0;
	}
	DWORD number = 0;
	if (!::WriteProcessMemory(hProcess, address, path, 0x60, &number))
	{
		return 0;
	}

	//
	HANDLE hRemoteThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, address, NULL, NULL);
	if (NULL == hRemoteThread)
	{
		return 0;
	}

	::WaitForSingleObject(hRemoteThread, INFINITE);

	DWORD lpExitCode = -1;
	::GetExitCodeThread(hRemoteThread, &lpExitCode);//退出码为LoadLibrary返回的写入的函数的地址
	if (lpExitCode != -1) 
	{
		hDllMod = (HMODULE)lpExitCode;
	}

	VirtualFreeEx(hProcess, address, 0x60, MEM_DECOMMIT);

	::CloseHandle(hRemoteThread);
	hRemoteThread = NULL;

	return 1;
}

int InjectionAndUnloading::remoteThreadUninstall(DWORD pid, wchar_t *path)
{
	if (hDllMod == NULL)
	{
		return 0;
	}

	//
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess == NULL)
	{
		return 0;
	}

	//
	HANDLE hRemoteThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, hDllMod, NULL, NULL);
	if (NULL == hRemoteThread)
	{
		return 0;
	}

	::WaitForSingleObject(hRemoteThread, INFINITE);

	DWORD lpExitCode = -1;
	::GetExitCodeThread(hRemoteThread, &lpExitCode);

	::CloseHandle(hRemoteThread);
	hRemoteThread = NULL;

	hDllMod = NULL;

	return 1;
}

void InjectionAndUnloading::operating(HWND hDlg, int status)
{
	wchar_t wPid[0x10] = { 0 };
	wchar_t wPath[0x60] = { 0 };
	HWND pathEdit = ::GetDlgItem(hDlg, IDC_EDIT_DELL_PATH);
	HWND pidEdit = ::GetDlgItem(hDlg, IDC_EDIT_PID);

	::SendMessage(pathEdit, WM_GETTEXT, 0x60, (LPARAM)wPath);
	::SendMessage(pidEdit, WM_GETTEXT, 0x10, (LPARAM)wPid);

	if (!wcslen(wPid) || !wcslen(wPath))
	{
		return;
	}

	//
	HWND hRemoteThread = ::GetDlgItem(hDlg, IDC_CHECK_REMOTE_THREAD);
	HWND hMemoryWrite = ::GetDlgItem(hDlg, IDC_CHECK_MEMORY_WRITE);
	HWND hProcessLoading = ::GetDlgItem(hDlg, IDC_CHECK_PROCESS_LOADING);
	HWND hInput = ::GetDlgItem(hDlg, IDC_CHECK_INPUT);

	int rtStatus = ::SendMessage(hRemoteThread, BM_GETCHECK, NULL, NULL);
	int mwStatus = ::SendMessage(hMemoryWrite, BM_GETCHECK, NULL, NULL);
	int plStatus = ::SendMessage(hProcessLoading, BM_GETCHECK, NULL, NULL);
	int iStatus  = ::SendMessage(hInput, BM_GETCHECK, NULL, NULL);

	if (rtStatus)
	{
		DWORD dPid = 0;
		swscanf_s(wPid, L"%d", &dPid);
		if (dPid)
		{
			if (status == 1)
			{
				if (remoteThreadInjection(dPid, wPath))
				{
					MessageBox(NULL, L"注入成功", L"Message", MB_OK);
				}
			}
			else if(status == 2)
			{
				if (remoteThreadUninstall(dPid, wPath))
				{
					MessageBox(NULL, L"卸载成功", L"Message", MB_OK);
				}
			}
			
		}

	}

	if (mwStatus)
	{

	}

	if (plStatus)
	{

	}

	if (iStatus)
	{

	}
}
