#pragma once
class InjectionAndUnloading
{
private:
	HINSTANCE peInstance;
	HMODULE hDllMod;
public:
	InjectionAndUnloading();
	~InjectionAndUnloading();
private:
	void selectFile(HWND hDlg);
	int  remoteThreadInjection(DWORD pid, wchar_t *path);
	int remoteThreadUninstall(DWORD pid, wchar_t *path);
	void operating(HWND hDlg,int status);
public:
	void createDialog(HINSTANCE PeInstance);

	friend INT_PTR CALLBACK injectionDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

