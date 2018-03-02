//PEParsing.cpp: 定义应用程序的入口点。
//
#include "stdafx.h"
#include "PEParsing.h"

HINSTANCE isInstance;

extern void CreatePEDialog(HINSTANCE thisInstance, HWND hDlg);

INT_PTR CALLBACK DlgProcAbout(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
void initProcessListView(HWND hDlg);
void enumProcess(HWND hListProcess);
void initProcessListViewBOTM(HWND hDlg);
void enumModules(HWND hListProcess, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

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

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_HOME_BOX), NULL, (DLGPROC)DlgProc);

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
			CreatePEDialog(isInstance,hDlg);
			break;
		case IDC_BUTTON_ABOUT:
			DialogBox(isInstance, MAKEINTRESOURCE(IDD_DIALOG_ABOUT), NULL, (DLGPROC)DlgProcAbout);
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
				enumModules(GetDlgItem(hDlg, IDC_LIST_TOP), wParam, lParam);
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
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
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
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

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

//获取某个liestview中某列值
void enumModules(HWND hListProcess, WPARAM wParam, LPARAM lParam) {
	DWORD dwRowId;
	TCHAR *szPid = NULL;
	szPid = (TCHAR*)malloc(sizeof(TCHAR) * 0x60);
	if (szPid == NULL) {
		return;
	}
	LV_ITEM lv;

	memset(&lv, 0, sizeof(LV_ITEM));
	memset(szPid, 0, sizeof(TCHAR) * 0x60);

	dwRowId = SendMessage(hListProcess, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
	if (dwRowId == -1) {
		MessageBox(hListProcess, L"请选择进程", L"错误", MB_OK);
	}
	else {
		lv.iSubItem = 1;		//第几列
		lv.pszText = szPid;		//指定存储查询结果的缓冲区
		lv.cchTextMax = 0x20;	//制定缓冲区大小
		SendMessage(hListProcess, LVM_GETITEMTEXT, dwRowId, (DWORD)&lv);
		//MessageBox(hListProcess, L"ss", L"PID", MB_OK);
		OutputDebugString(szPid);
	}
}


