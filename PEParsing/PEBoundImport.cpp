#include "stdafx.h"
#include "PEBoundImport.h"


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
		/*
		DWORD boundImportRVA = petc.getDWValue((pei->pointer + directoryLocat + 88), 4);
		if (boundImportRVA != 0)
		{
		DWORD boundImportFOA = petc.rvaTofoa(pei->pointer, boundImportRVA);

		for (int i = 0; ; i++)
		{
		memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);
		DWORD timeDateStamp = petc.getDWValue(pei->pointer + boundImportFOA , 4);
		WORD offsetModulNmae = petc.getDWValue(pei->pointer + boundImportFOA + 4, 2);
		WORD numberOfModuleForwarderRefs = petc.getDWValue(pei->pointer + boundImportFOA + 6, 2);
		if (offsetModulNmae == 0)
		{
		break;
		}
		DWORD nameRVA = offsetModulNmae + boundImportFOA;
		DWORD nameFOA = petc.rvaTofoa(pei->pointer, nameRVA);

		wsprintf(temporaryBUffer, L"%08X", timeDateStamp);
		petc.getCharPointer(pei->pointer + nameFOA, temporaryBUffer + 9, 0);

		pei->insterList(importHwnd, i, temporaryBUffer + 9, L"FFFFFFFF", temporaryBUffer, L"FFFFFFFF");
		if (numberOfModuleForwarderRefs == 0)
		{
		numberOfModuleForwarderRefs = 1;
		}
		boundImportFOA = boundImportFOA + (numberOfModuleForwarderRefs * 8);
		}
		}
		*/
		break;
	}
	case WM_NOTIFY:
	{
		/*NMHDR* pNMHDR = (NMHDR*)lParam;
		if (pNMHDR->code == NM_CLICK) {
			switch (wParam)
			{
			case IDC_LIST_PE_IMPORT:
				pei->enumModules(hDlg, wParam, lParam);
				break;
			}
		}*/
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


	SendMessage(hListProcessBound, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	SendMessage(hListProcessMembers, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
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

}

void PEBoundImport::insterList(HWND blockHwnd, int number, TCHAR *characterOne, TCHAR *characterTWO, TCHAR *characterThree, TCHAR *characterFour)
{

}
