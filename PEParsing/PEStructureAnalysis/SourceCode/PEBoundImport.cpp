#include "..\..\stdafx.h"
#include "..\index\PEBoundImport.h"
#include "..\..\Tools\PE\PeToolsClass.h"

#include <time.h> 

PEBoundImport::PEBoundImport()
{
}


PEBoundImport::~PEBoundImport()
{
}

void PEBoundImport::creadPEBoundImportDialog(HINSTANCE PeInstance, BYTE *fpointer)
{
	pointer = fpointer;
	peInstance = PeInstance;
	if (peInstance != NULL)
	{
		DialogBox(peInstance, MAKEINTRESOURCE(IDD_DIALOG_PE_BOUND_IMPORT), NULL, (DLGPROC)peBoundImportDialog);
	}
}

INT_PTR CALLBACK peBoundImportDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	extern PEBoundImport *pebi;
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
		pebi->initList(hDlg);
		if (pebi->pointer != NULL)
		{
			PeToolsClass petc;
			TCHAR *temporaryBUffer = NULL;
			temporaryBUffer = (TCHAR*)malloc(sizeof(TCHAR) * 0x100);
			if (temporaryBUffer == NULL)
			{
				return false;
			}
			memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);
			DWORD platform = petc.getApplicationSize(pebi->pointer);
			DWORD directoryLocat = 0;
			if (platform == 0x014C)
			{
				directoryLocat = petc.getDWValue((pebi->pointer + 60), 4) + 24 + 96;
			}
			else if (platform == 0x8664)
			{
				directoryLocat = petc.getDWValue((pebi->pointer + 60), 4) + 24 + 112;
			}
			DWORD boundImportRVA = petc.getDWValue((pebi->pointer + directoryLocat + 88), 4);

			if (boundImportRVA != 0)
			{
				DWORD boundImportFOA = petc.rvaTofoa(pebi->pointer, boundImportRVA);
				HWND boundImportHwnd = GetDlgItem(hDlg, IDC_LIST_BOUND_IMPORT);
				DWORD fistBoundImportFOA = boundImportFOA;
				for (int i = 0,j=0; ; i++)
				{
					memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);
					DWORD timeDateStamp = petc.getDWValue(pebi->pointer + boundImportFOA, 4);
					WORD offsetModulNmae = petc.getDWValue(pebi->pointer + boundImportFOA + 4, 2);
					WORD numberOfModuleForwarderRefs = petc.getDWValue(pebi->pointer + boundImportFOA + 6, 2);
					if (offsetModulNmae == 0 && timeDateStamp == 0)
					{
						break;
					}
					DWORD nameRVA = offsetModulNmae + fistBoundImportFOA;
					DWORD nameFOA = petc.rvaTofoa(pebi->pointer, nameRVA);

					wsprintf(temporaryBUffer, L"%08X", timeDateStamp);
					wsprintf(temporaryBUffer + 9, L"%04X", offsetModulNmae);
					wsprintf(temporaryBUffer + 18, L"%08X", numberOfModuleForwarderRefs);

					time_t rawtime = timeDateStamp;
					struct tm timeptr;
					localtime_s(&timeptr,&rawtime);
					wcsftime(temporaryBUffer + 27, sizeof(TCHAR) * 0x20, L"%Y-%m-%d %H:%M:%S", &timeptr);

					petc.getCharPointer(pebi->pointer + nameFOA, temporaryBUffer + 60, 0);

					pebi->insterList(boundImportHwnd, i, temporaryBUffer + 60, temporaryBUffer, temporaryBUffer + 27, temporaryBUffer + 9, temporaryBUffer + 18);
					if (numberOfModuleForwarderRefs == 0)
					{
						numberOfModuleForwarderRefs = 1;
					}
					*(pebi->boundFOA + j) = boundImportFOA;
					boundImportFOA = boundImportFOA + (numberOfModuleForwarderRefs * 8);
					j++;
				}
			}

			if (temporaryBUffer != NULL)
			{
				free(temporaryBUffer);
				temporaryBUffer = NULL;
			}
		}

		break;
	}
	case WM_NOTIFY:
	{
		NMHDR* pNMHDR = (NMHDR*)lParam;
		if (pNMHDR->code == NM_CLICK) {
			switch (wParam)
			{
			case IDC_LIST_BOUND_IMPORT:
				pebi->enumModules(hDlg, wParam, lParam);
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
	}

	return false;
}

void PEBoundImport::initList(HWND hDlg)
{
	LV_COLUMN lv;
	HWND hListProcessBound;
	HWND hListProcessMembers;

	memset(&lv, 0, sizeof(LV_COLUMN));
	hListProcessBound = GetDlgItem(hDlg, IDC_LIST_BOUND_IMPORT);
	hListProcessMembers = GetDlgItem(hDlg, IDC_LIST_BOUND_IMPORT_MEMBERS);


	SendMessage(hListProcessBound, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SendMessage(hListProcessMembers, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
	lv.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;

	lv.cx = 190;
	lv.pszText = TEXT("DLL Name");
	lv.iSubItem = 0;
	ListView_InsertColumn(hListProcessBound, 0, &lv);

	lv.cx = 110;
	lv.pszText = TEXT("TimeDateStamp");
	lv.iSubItem = 1;
	ListView_InsertColumn(hListProcessBound, 1, &lv);

	lv.cx = 210;
	lv.pszText = TEXT("Calendar");
	lv.iSubItem = 2;
	ListView_InsertColumn(hListProcessBound, 2, &lv);

	lv.cx = 110;
	lv.pszText = TEXT("OffsetModuleName");
	lv.iSubItem = 3;
	ListView_InsertColumn(hListProcessBound, 3, &lv);

	lv.cx = 130;
	lv.pszText = TEXT("NumberOfModuleForwarderRefs");
	lv.iSubItem = 4;
	ListView_InsertColumn(hListProcessBound, 4, &lv);

	lv.cx = 190;
	lv.pszText = TEXT("DLL Name");
	lv.iSubItem = 0;
	ListView_InsertColumn(hListProcessMembers, 0, &lv);

	lv.cx = 110;
	lv.pszText = TEXT("TimeDateStamp");
	lv.iSubItem = 1;
	ListView_InsertColumn(hListProcessMembers, 1, &lv);

	lv.cx = 200;
	lv.pszText = TEXT("Calendar");
	lv.iSubItem = 2;
	ListView_InsertColumn(hListProcessMembers, 2, &lv);

	lv.cx = 110;
	lv.pszText = TEXT("OffsetModuleName");
	lv.iSubItem = 3;
	ListView_InsertColumn(hListProcessMembers, 3, &lv);

	lv.cx = 130;
	lv.pszText = TEXT("Reserved(±£¡Ù)");
	lv.iSubItem = 4;
	ListView_InsertColumn(hListProcessMembers, 4, &lv);
}

void PEBoundImport::enumModules(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	HWND blockHwnd = GetDlgItem(hDlg, IDC_LIST_BOUND_IMPORT);
	DWORD dwRowId;
	TCHAR szPid[0x10];
	LV_ITEM lv;

	memset(&lv, 0, sizeof(LV_ITEM));

	dwRowId = SendMessage(blockHwnd, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
	if (dwRowId != -1)
	{
		lv.pszText = szPid;
		lv.cchTextMax = 0x10;

		memset(szPid, 0, sizeof(TCHAR) * 0x10);
		lv.iSubItem = 4;
		SendMessage(blockHwnd, LVM_GETITEMTEXT, dwRowId, (DWORD)&lv);
		int number = 0;
		swscanf_s(szPid, L"%x", &number);

		if (number != 0)
		{
			HWND memberHwnd = GetDlgItem(hDlg, IDC_LIST_BOUND_IMPORT_MEMBERS);
			disMembers(memberHwnd, number);
		}
	}
}

void PEBoundImport::disMembers(HWND membersHwnd, DWORD number)
{
	if (boundFOA[0] != 0)
	{
		PeToolsClass petc;
		TCHAR *temporaryBUffer = NULL;
		temporaryBUffer = (TCHAR*)malloc(sizeof(TCHAR) * 0x100);
		if (temporaryBUffer == NULL)
		{
			return;
		}
		memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);

		DWORD fistBoundImportFOA = boundFOA[0];
		DWORD boundImportFOA = boundFOA[number];
		for (int i = 0, j = 0; ; i++)
		{
			memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);
			DWORD timeDateStamp = petc.getDWValue(pointer + boundImportFOA, 4);
			WORD offsetModulNmae = petc.getDWValue(pointer + boundImportFOA + 4, 2);
			WORD numberOfModuleForwarderRefs = petc.getDWValue(pointer + boundImportFOA + 6, 2);
			if (offsetModulNmae == 0 && timeDateStamp == 0)
			{
				break;
			}
			DWORD nameRVA = offsetModulNmae + fistBoundImportFOA;
			DWORD nameFOA = petc.rvaTofoa(pointer, nameRVA);

			wsprintf(temporaryBUffer, L"%08X", timeDateStamp);
			wsprintf(temporaryBUffer + 9, L"%04X", offsetModulNmae);
			wsprintf(temporaryBUffer + 18, L"%08X", numberOfModuleForwarderRefs);

			time_t rawtime = timeDateStamp;
			struct tm timeptr;
			localtime_s(&timeptr, &rawtime);
			wcsftime(temporaryBUffer + 27, sizeof(TCHAR) * 0x20, L"%Y-%m-%d %H:%M:%S", &timeptr);

			petc.getCharPointer(pointer + nameFOA, temporaryBUffer + 60, 0);

			insterList(membersHwnd, i, temporaryBUffer + 60, temporaryBUffer, temporaryBUffer + 27, temporaryBUffer + 9, temporaryBUffer + 18);
		}

		if (temporaryBUffer != NULL)
		{
			free(temporaryBUffer);
			temporaryBUffer = NULL;
		}


	}
	
}

void PEBoundImport::insterList(HWND blockHwnd, int number, TCHAR *characterOne, TCHAR *characterTWO, TCHAR *characterThree, TCHAR *characterFour, TCHAR *characterFive)
{
	LV_ITEM vitem;
	memset(&vitem, 0, sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;

	vitem.pszText = characterOne;
	vitem.iItem = number;
	vitem.iSubItem = 0;
	ListView_InsertItem(blockHwnd, &vitem);

	vitem.pszText = characterTWO;
	vitem.iItem = number;
	vitem.iSubItem = 1;
	ListView_SetItem(blockHwnd, &vitem);

	if (characterThree != NULL) {
		vitem.pszText = characterThree;
		vitem.iItem = number;
		vitem.iSubItem = 2;
		ListView_SetItem(blockHwnd, &vitem);
	}

	if (characterFour != NULL) {
		vitem.pszText = characterFour;
		vitem.iItem = number;
		vitem.iSubItem = 3;
		ListView_SetItem(blockHwnd, &vitem);
	}

	if (characterFive != NULL) {
		vitem.pszText = characterFive;
		vitem.iItem = number;
		vitem.iSubItem = 4;
		ListView_SetItem(blockHwnd, &vitem);
	}
}
