#include "stdafx.h"
#include "PEImport.h"
#include "PeToolsClass.h"


PEImport::PEImport()
{
}


PEImport::~PEImport()
{
}

void PEImport::creadPEImportDialog(HINSTANCE PeInstance, BYTE *fpointer)
{
	pointer = fpointer;
	peInstance = PeInstance;
	if (peInstance != NULL)
	{
		DialogBox(peInstance, MAKEINTRESOURCE(IDD_DIALOG_PE_IMPORT), NULL, (DLGPROC)peImportDialog);
	}
}

INT_PTR CALLBACK peImportDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	extern PEImport *pei;
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
		pei->initList(hDlg);
		if (pei->pointer != NULL)
		{
			PeToolsClass petc;
			TCHAR *temporaryBUffer = NULL;
			temporaryBUffer = (TCHAR*)malloc(sizeof(TCHAR) * 0x100);
			if (temporaryBUffer == NULL)
			{
				return false;
			}
			memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);

			DWORD directoryLocat = petc.getDWValue((pei->pointer + 60), 4) + 24 + 96;
			DWORD importRVA = petc.getDWValue((pei->pointer + directoryLocat + 8), 4);
			if (importRVA == 0)
			{
				break;
			}
			DWORD importFOA = petc.rvaTofoa(pei->pointer, importRVA);

			HWND importHwnd = GetDlgItem(hDlg, IDC_LIST_PE_IMPORT);
			for (int i = 0; ; i++)
			{
				DWORD originalFirstThunkRVA = petc.getDWValue((pei->pointer + importFOA + (i * 20)), 4);
				DWORD firstThunkRVA = petc.getDWValue((pei->pointer + importFOA + 16 + (i * 20)), 4);
				if (originalFirstThunkRVA == 0 && firstThunkRVA == 0)
				{
					break;
				}
				DWORD timeDateStamp = petc.getDWValue((pei->pointer + importFOA + 4 + (i * 20)), 4);
				if (timeDateStamp == -1)
				{
					continue;
				}
				//DWORD nameRVA = petc.getDWValue((pei->pointer + importFOA + 12 + (i * 20)), 4);
				DWORD nameFOA = petc.rvaTofoa(pei->pointer, petc.getDWValue((pei->pointer + importFOA + 12 + (i * 20)), 4));

				memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);
				wsprintf(temporaryBUffer, L"%08X", firstThunkRVA);
				wsprintf(temporaryBUffer + 9, L"%08X", timeDateStamp);
				wsprintf(temporaryBUffer + 18, L"%08X", originalFirstThunkRVA);
				petc.getCharPointer((pei->pointer + nameFOA), temporaryBUffer + 27, 0);

				pei->insterList(importHwnd, i, temporaryBUffer + 27, temporaryBUffer + 18, temporaryBUffer + 9, temporaryBUffer);
			}

			/*DWORD boundImportRVA = petc.getDWValue((pei->pointer + directoryLocat + 88), 4);
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

					boundImportFOA = boundImportFOA + (numberOfModuleForwarderRefs * 8);
				}
			}*/
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
			case IDC_LIST_PE_IMPORT:
				pei->enumModules(hDlg, wParam, lParam);
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

void PEImport::initList(HWND hDlg)
{
	LV_COLUMN lv;
	HWND hListProcessImport;
	HWND hListProcessIAT;
	HWND hListProcessINT;

	memset(&lv, 0, sizeof(LV_COLUMN));
	hListProcessImport = GetDlgItem(hDlg, IDC_LIST_PE_IMPORT);
	hListProcessIAT = GetDlgItem(hDlg, IDC_LIST_IMPORT_IAT);
	hListProcessINT = GetDlgItem(hDlg, IDC_LIST_IMPORT_INT);


	SendMessage(hListProcessImport, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	SendMessage(hListProcessIAT, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	SendMessage(hListProcessINT, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
	lv.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;

	lv.cx = 200;
	lv.pszText = TEXT("Name");
	lv.iSubItem = 0;
	ListView_InsertColumn(hListProcessImport, 0, &lv);

	lv.cx = 106;
	lv.pszText = TEXT("OriginalFirstThunk");
	lv.iSubItem = 1;
	ListView_InsertColumn(hListProcessImport, 1, &lv);

	lv.pszText = TEXT("TimeDateStamp");
	lv.iSubItem = 2;
	ListView_InsertColumn(hListProcessImport, 2, &lv);

	lv.pszText = TEXT("FirstThunk");
	lv.iSubItem = 3;
	ListView_InsertColumn(hListProcessImport, 3, &lv);

	lv.cx = 55;
	lv.pszText = TEXT("Index");
	lv.iSubItem = 0;
	ListView_InsertColumn(hListProcessINT, 0, &lv);

	lv.cx = 100;
	lv.pszText = TEXT("Hint");
	lv.iSubItem = 1;
	ListView_InsertColumn(hListProcessINT, 1, &lv);

	lv.cx = 200;
	lv.pszText = TEXT("Name/Ordinal");
	lv.iSubItem = 2;
	ListView_InsertColumn(hListProcessINT, 2, &lv);

	lv.cx = 55;
	lv.pszText = TEXT("Index");
	lv.iSubItem = 0;
	ListView_InsertColumn(hListProcessIAT, 0, &lv);

	lv.cx = 150;
	lv.pszText = TEXT("FunctionAddress");
	lv.iSubItem = 1;
	ListView_InsertColumn(hListProcessIAT, 1, &lv);

	lv.cx = 150;
	lv.pszText = TEXT("FunctionAddressOffset");
	lv.iSubItem = 2;
	ListView_InsertColumn(hListProcessIAT, 2, &lv);
}

void PEImport::enumModules(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	HWND blockHwnd = GetDlgItem(hDlg, IDC_LIST_PE_IMPORT);
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
		lv.iSubItem = 1;
		SendMessage(blockHwnd, LVM_GETITEMTEXT, dwRowId, (DWORD)&lv);
		int intValue = 0;
		swscanf_s(szPid, L"%x", &intValue);

		memset(szPid, 0, sizeof(TCHAR) * 0x10);
		lv.iSubItem = 3;
		SendMessage(blockHwnd, LVM_GETITEMTEXT, dwRowId, (DWORD)&lv);
		int iatValue = 0;
		swscanf_s(szPid, L"%x", &iatValue);

		if (iatValue != 0 && intValue != 0)
		{
			disIatIntMembers(hDlg, iatValue, intValue);
		}
	}
}

void PEImport::disIatIntMembers(HWND membersHwnd, DWORD iatRVA, DWORD intRVA)
{
	PeToolsClass petc;
	HWND iatHwnd = GetDlgItem(membersHwnd, IDC_LIST_IMPORT_IAT);
	HWND intHwnd = GetDlgItem(membersHwnd, IDC_LIST_IMPORT_INT);
	ListView_DeleteAllItems(iatHwnd);
	ListView_DeleteAllItems(intHwnd);
	DWORD firstThunkFOA = petc.rvaTofoa(pointer, iatRVA);
	DWORD originalFirstThynkFOA = petc.rvaTofoa(pointer, intRVA);

	TCHAR *temporaryBUffer = NULL;
	temporaryBUffer = (TCHAR*)malloc(sizeof(TCHAR) * 0x100);
	if (temporaryBUffer == NULL)
	{
		return;
	}
	memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);

	for (int i = 0; ; i++)
	{
		DWORD thunkdataINT = petc.getDWValue(pointer + originalFirstThynkFOA + (i * 4), 4);
		if (thunkdataINT == 0)
		{
			break;
		}
		int sd = thunkdataINT & 0x80000000;
		if ((thunkdataINT & 0x80000000))
		{
			memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);
			wsprintf(temporaryBUffer, L"%d", i + 1);
			wsprintf(temporaryBUffer + 9, L"%08X", (thunkdataINT & 0x7fffffff));

			insterList(intHwnd, i, temporaryBUffer, L"--", temporaryBUffer + 9, NULL);
		}
		else
		{
			memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);
			DWORD byNameFOA = petc.rvaTofoa(pointer, thunkdataINT);
			WORD hint = petc.getDWValue(pointer + byNameFOA, 2);
			wsprintf(temporaryBUffer, L"%d", i + 1);
			wsprintf(temporaryBUffer + 9, L"%04X", hint);
			petc.getCharPointer(pointer + byNameFOA + 2, temporaryBUffer + 18, 0);

			insterList(intHwnd, i, temporaryBUffer, temporaryBUffer + 9, temporaryBUffer + 18, NULL);
		}

	}

	for (int i = 0; ; i++)
	{
		DWORD thunkdataIAT = petc.getDWValue(pointer + firstThunkFOA + (i * 4), 4);
		if (thunkdataIAT == 0)
		{
			break;
		}
		DWORD thunkdataIATFOA = petc.rvaTofoa(pointer, thunkdataIAT);
		memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);
		wsprintf(temporaryBUffer, L"%d", i + 1);
		wsprintf(temporaryBUffer + 9, L"%08X", thunkdataIAT);
		wsprintf(temporaryBUffer + 18, L"%08X", thunkdataIATFOA);
		insterList(iatHwnd, i, temporaryBUffer, temporaryBUffer + 9, temporaryBUffer + 18, NULL);
	}

	if (temporaryBUffer != NULL)
	{
		free(temporaryBUffer);
		temporaryBUffer = NULL;
	}
}

void PEImport::insterList(HWND controlHwnd, int number, TCHAR *characterOne, TCHAR *characterTWO, TCHAR *characterThree, TCHAR *characterFour)
{
	LV_ITEM vitem;
	memset(&vitem, 0, sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;

	vitem.pszText = characterOne;
	vitem.iItem = number;
	vitem.iSubItem = 0;
	ListView_InsertItem(controlHwnd, &vitem);

	vitem.pszText = characterTWO;
	vitem.iItem = number;
	vitem.iSubItem = 1;
	ListView_SetItem(controlHwnd, &vitem);

	if (characterThree != NULL) {
		vitem.pszText = characterThree;
		vitem.iItem = number;
		vitem.iSubItem = 2;
		ListView_SetItem(controlHwnd, &vitem);
	}

	if (characterFour != NULL) {
		vitem.pszText = characterFour;
		vitem.iItem = number;
		vitem.iSubItem = 3;
		ListView_SetItem(controlHwnd, &vitem);
	}
}
