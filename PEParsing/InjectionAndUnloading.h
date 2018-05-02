#pragma once
class InjectionAndUnloading
{
private:
	HINSTANCE peInstance = NULL;
public:
	InjectionAndUnloading();
	~InjectionAndUnloading();
private:
	void selectFile(HWND hDlg);
	int remoteThreadInjection(DWORD pid, wchar_t *path);
public:
	void createDialog(HINSTANCE PeInstance);

	friend INT_PTR CALLBACK injectionDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

