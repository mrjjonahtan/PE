//PEParsing.cpp: 定义应用程序的入口点。
//
#include "stdafx.h"
#include "PEEncryptedshell\PEEncryptedshell.h"
#include "ThreadInjection\InjectionAndUnloading.h"
#include "Tools\PE\PeToolsClass.h"
#include "PEParsing.h"


HINSTANCE isInstance;

extern void CreatePEDialog(HINSTANCE thisInstance, HWND hDlg);

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
void initProcessListView(HWND hDlg);
void enumProcess(HWND hListProcess);
void initProcessListViewBOTM(HWND hDlg);
void enumModules(HWND hDlg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProcAbout(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	isInstance = hInstance;
	//初始化通用控件
	INITCOMMONCONTROLSEX iccte;
	iccte.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccte.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&iccte);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_HOME_BOX), NULL, DlgProc);

	return 0;
}
//主窗口回掉
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_INITDIALOG:
		initProcessListView(hDlg);
		initProcessListViewBOTM(hDlg);
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_PE:
			CreatePEDialog(isInstance, hDlg);
			break;
		case IDC_BUTTON_ENCRYPTEDSHELL:
			peencryptedshell = new PEEncryptedshell();
			if (peencryptedshell != NULL)
			{
				peencryptedshell->createDialog(isInstance);
				delete(peencryptedshell);
			}
			break;
		case IDC_BUTTON_INJECTION_UNINSTALL:
		{
			injectunloading = new InjectionAndUnloading();
			if (injectunloading != NULL)
			{
				injectunloading->createDialog(isInstance);
				delete(injectunloading);
			}
		}
		break;
		case IDC_BUTTON_ABOUT:
			DialogBox(isInstance, MAKEINTRESOURCE(IDD_DIALOG_ABOUT), NULL, DlgProcAbout);
			break;
		case IDC_BUTTON_OUT:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	case WM_NOTIFY:
	{
		NMHDR* pNMHDR = (NMHDR*)lParam;
		if (pNMHDR->code == NM_CLICK) {
			switch (wParam)
			{
			case IDC_LIST_TOP:
				enumModules(hDlg, wParam, lParam);
				break;
			}
		}
		break;
	}
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	}

	return false;
}

//关于回掉
INT_PTR CALLBACK DlgProcAbout(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage)
	{
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_ABOUT_CLOSE:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return false;
}

void initProcessListView(HWND hDlg) {
	LV_COLUMN lv;
	HWND hListProcess;

	//初始化
	memset(&lv, 0, sizeof(LV_COLUMN));
	//获得IDC_LIST1句柄
	hListProcess = GetDlgItem(hDlg, IDC_LIST_TOP);

	//设置选中行
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	lv.pszText = TEXT("进程");				//列名称
	lv.cx = 300;						//列宽
	lv.iSubItem = 0;					//第几个列
										//SendMessage(hListProcess, LVM_INSERTCOLUMN, 0, (DWORD)&lv);
	ListView_InsertColumn(hListProcess, 0, &lv);

	lv.pszText = TEXT("PID");				//列名称
	lv.cx = 140;						//列宽
	lv.iSubItem = 1;					//第几个列
										//SendMessage(hListProcess, LVM_INSERTCOLUMN, 1, (DWORD)&lv);
	ListView_InsertColumn(hListProcess, 1, &lv);

	lv.pszText = TEXT("镜像基址");				//列名称
	lv.cx = 130;						//列宽
	lv.iSubItem = 2;					//第几个列
										//SendMessage(hListProcess, LVM_INSERTCOLUMN, 2, (DWORD)&lv);
	ListView_InsertColumn(hListProcess, 2, &lv);

	lv.pszText = TEXT("镜像大小");				//列名称
	lv.cx = 145;						//列宽
	lv.iSubItem = 3;					//第几个列
										//SendMessage(hListProcess, LVM_INSERTCOLUMN, 3, (DWORD)&lv);
	ListView_InsertColumn(hListProcess, 3, &lv);

	enumProcess(hListProcess);
}

//设置listview列
void initProcessListViewBOTM(HWND hDlg) {
	LV_COLUMN lv;
	HWND hListProcess;

	//初始化
	memset(&lv, 0, sizeof(LV_COLUMN));
	//获得IDC_LIST1句柄
	hListProcess = GetDlgItem(hDlg, IDC_LIST_BOTM);

	//设置选中行
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
	lv.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;

	lv.pszText = TEXT("模块名称");				//列名称
	lv.cx = 355;						//列宽
	lv.iSubItem = 0;					//第几个列
										//SendMessage(hListProcess, LVM_INSERTCOLUMN, 0, (DWORD)&lv);
	ListView_InsertColumn(hListProcess, 0, &lv);

	lv.pszText = TEXT("模块位置");				//列名称
	lv.cx = 380;						//列宽
	lv.iSubItem = 1;					//第几个列
										//SendMessage(hListProcess, LVM_INSERTCOLUMN, 1, (DWORD)&lv);
	ListView_InsertColumn(hListProcess, 1, &lv);

}

//获得进程 listview填充数据
void enumProcess(HWND hListProcess) {
	LV_ITEM vitem;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	memset(&vitem, 0, sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;
	/*
	vitem.pszText = TEXT("exe.exe");
	vitem.iItem = 0;
	vitem.iSubItem = 0;
	ListView_InsertItem(hListProcess, &vitem);
	*/

	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		MessageBox(hListProcess, L"CreateToolHhelp32Snapshot调用失败", L"错误", MB_OK);
	}
	else {
		BOOL bMore = Process32First(hProcessSnap, &pe32);
		int i = 0;
		while (bMore) {

			vitem.pszText = pe32.szExeFile;//进程名称
			vitem.iItem = i;
			vitem.iSubItem = 0;
			ListView_InsertItem(hListProcess, &vitem);

			char szStr[20] = { '\0' };
			_ultoa_s(pe32.th32ProcessID, szStr, 10);
			WCHAR wszClassName[256];
			memset(wszClassName, 0, sizeof(wszClassName));
			MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName, sizeof(wszClassName) / sizeof(wszClassName[0]));

			vitem.pszText = LPWSTR(wszClassName);//进程ID号 pe32.th32ProcessID
			vitem.iItem = i;
			vitem.iSubItem = 1;
			ListView_SetItem(hListProcess, &vitem);

			bMore = Process32Next(hProcessSnap, &pe32);
			i = i + 1;
		}
	}
	CloseHandle(hProcessSnap);

}

//显示
void showModule(BYTE *pointer, HWND hDlg)
{
	PeToolsClass petc;
	DWORD lfanew = 0;
	DWORD imagebase = 0;
	DWORD importLocation = 0;

	TCHAR *temporaryBUffer = NULL;
	temporaryBUffer = (TCHAR*)malloc(sizeof(TCHAR) * 0x100);
	if (temporaryBUffer == NULL)
	{
		return ;
	}
	memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);

	DWORD platform = petc.getApplicationSize(pointer);
	DWORD directoryLocat = 0;
	if (platform == 0x014C)
	{
		directoryLocat = petc.getDWValue((pointer + 60), 4) + 24 + 96;
	}
	else if (platform == 0x8664)
	{
		directoryLocat = petc.getDWValue((pointer + 60), 4) + 24 + 112;
	}

	DWORD importRVA = petc.getDWValue((pointer + directoryLocat + 8), 4);
	if (importRVA == 0)
	{
		return;
	}
	DWORD importFOA = petc.rvaTofoa(pointer, importRVA);

	//
	HWND importHwnd = GetDlgItem(hDlg, IDC_LIST_BOTM);
	if (importHwnd == NULL)
	{
		return;
	}
	//ListView_DeleteAllItems(importHwnd);
	//
	LV_ITEM vitem;
	memset(&vitem, 0, sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;

	for (int i = 0; ; i++)
	{

		//
		DWORD originalFirstThunkRVA = petc.getDWValue((pointer + importFOA + (i * 20)), 4);
		DWORD firstThunkRVA = petc.getDWValue((pointer + importFOA + 16 + (i * 20)), 4);
		if (originalFirstThunkRVA == 0 && firstThunkRVA == 0)
		{
			break;
		}
		DWORD timeDateStamp = petc.getDWValue((pointer + importFOA + 4 + (i * 20)), 4);
		if (timeDateStamp == -1)
		{
			continue;
		}
		//DWORD nameRVA = petc.getDWValue((pei->pointer + importFOA + 12 + (i * 20)), 4);
		DWORD nameFOA = petc.rvaTofoa(pointer, petc.getDWValue((pointer + importFOA + 12 + (i * 20)), 4));

		memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);
		//wsprintf(temporaryBUffer, L"%08X", firstThunkRVA);
		//wsprintf(temporaryBUffer + 9, L"%08X", timeDateStamp);
		wsprintf(temporaryBUffer + 18, L"%08X", originalFirstThunkRVA);
		petc.getCharPointer((pointer + nameFOA), temporaryBUffer + 27, 0);
		
		//
		vitem.pszText = (temporaryBUffer + 27);
		vitem.iItem = i;
		vitem.iSubItem = 0;
		ListView_InsertItem(importHwnd, &vitem);

		vitem.pszText = (temporaryBUffer + 18);
		vitem.iItem = i;
		vitem.iSubItem = 1;
		ListView_SetItem(importHwnd, &vitem);

	}
	if (temporaryBUffer != NULL)
	{
		free(temporaryBUffer);
		temporaryBUffer = NULL;
	}

}

//获得进程中的exe
void getModule(DWORD Pid, HWND hDlg)
{
	FILE *openFile = NULL;
	BYTE *pointer = NULL;
	DWORD size = 0;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);

	if (hProcess == NULL)
	{
		return;
	}

	//
	wchar_t path[MAX_PATH + 1];
	if (!GetModuleFileNameEx(hProcess, NULL, path, MAX_PATH + 1))
	{
		return;
	}

	//
	_wfopen_s(&openFile, path, L"rb");
	if (openFile == NULL) {
		return;
	}
	if (fseek(openFile, 0L, SEEK_END)) {
		return;
	}

	size = ftell(openFile);
	if (size == 0) {
		return;
	}

	pointer = (BYTE*)malloc(sizeof(BYTE)*size);
	if (pointer == NULL) {
		return;
	}

	memset(pointer, 0L, sizeof(BYTE)*size);

	fseek(openFile, 0L, 0L);
	if (fread(pointer, sizeof(BYTE)*size, 1, openFile) <= 0) {
		return;
	}

	if (openFile != NULL) {
		fclose(openFile);
		openFile = NULL;
	}

	//
	showModule(pointer, hDlg);

	if (pointer != NULL)
	{
		free(pointer);
		pointer = NULL;
	}
}

//获取某个liestview中某列值
void enumModules(HWND hDlg, WPARAM wParam, LPARAM lParam) {
	HWND hListProcess = GetDlgItem(hDlg, IDC_LIST_TOP);
	DWORD dwRowId;
	TCHAR szPid[0x20];
	LV_ITEM lv;

	memset(&lv, 0, sizeof(LV_ITEM));
	memset(szPid, 0, sizeof(TCHAR) * 0x20);

	dwRowId = SendMessage(hListProcess, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
	if (dwRowId == -1) {
		MessageBox(hListProcess, L"请选择进程", L"错误", MB_OK);
	}
	else {
		lv.iSubItem = 1;		//第几列
		lv.pszText = szPid;		//指定存储查询结果的缓冲区
		lv.cchTextMax = 0x20;	//制定缓冲区大小
		SendMessage(hListProcess, LVM_GETITEMTEXT, dwRowId, (DWORD)&lv);
		//
		DWORD pid = 0;
		swscanf_s(szPid, L"%d", &pid);

		//clear
		ListView_DeleteAllItems(GetDlgItem(hDlg, IDC_LIST_BOTM));

		getModule(pid, hDlg);
		//OutputDebugString(szPid);
	}
}


