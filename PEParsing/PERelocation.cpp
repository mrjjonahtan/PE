#include "stdafx.h"
#include "PeToolsClass.h"
#include "PERelocation.h"


PERelocation::PERelocation()
{
}


PERelocation::~PERelocation()
{
	peInstance = NULL;
	pointer = NULL;
}

void PERelocation::creadPERelocationDialog(HINSTANCE PeInstance, BYTE *fpointer)
{

	pointer = fpointer;
	peInstance = PeInstance;
	if (peInstance != NULL)
	{
		DialogBox(peInstance, MAKEINTRESOURCE(IDD_DIALOG_PE_RELOCATION), NULL, (DLGPROC)peRelocationDialog);
	}
}

INT_PTR CALLBACK peRelocationDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	extern PERelocation *per;
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
		per->initList(hDlg);
		if (per->pointer != NULL)
		{
			PeToolsClass petc;
			TCHAR *temporaryBUffer = NULL;
			temporaryBUffer = (TCHAR*)malloc(sizeof(TCHAR) * 0x30);
			if (temporaryBUffer == NULL)
			{
				return false;
			}
			memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x30);
			DWORD platform = petc.getApplicationSize(per->pointer);
			DWORD directoryLocat = 0;
			if (platform == 0x014C)
			{
				directoryLocat = petc.getDWValue((per->pointer + 60), 4) + 24 + 96;
			}
			else if (platform == 0x8664)
			{
				directoryLocat = petc.getDWValue((per->pointer + 60), 4) + 24 + 112;
			}

			DWORD RelocationRVA = petc.getDWValue((per->pointer + directoryLocat + (10 * 4)), 4);
			if (RelocationRVA == 0)
			{
				break;
			}
			DWORD RelocationFOA = petc.rvaTofoa(per->pointer, RelocationRVA);
			HWND blockHwnd = GetDlgItem(hDlg, IDC_LIST_PE_RELOCATION_BLOCKS);
			int i = 0;
			while (true)
			{
				DWORD RelocationRVA = petc.getDWValue((per->pointer + RelocationFOA), 4);
				DWORD sizeofBlock = petc.getDWValue((per->pointer + RelocationFOA + 4), 4);
				if (RelocationRVA == 0 && sizeofBlock == 0)
				{
					break;
				}
				i++;
				memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x30);
				wsprintf(temporaryBUffer, L"%d", i);
				wsprintf(temporaryBUffer + 9, L"%08X", RelocationRVA);
				wsprintf(temporaryBUffer + 18, L"%08X", sizeofBlock);
				wsprintf(temporaryBUffer + 27, L"%08X", RelocationFOA);
				RelocationFOA = RelocationFOA + sizeofBlock;
				per->insterList(blockHwnd, i - 1, temporaryBUffer, temporaryBUffer + 27, temporaryBUffer + 9, temporaryBUffer + 18);
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
			case IDC_LIST_PE_RELOCATION_BLOCKS:
				per->enumModules(hDlg, wParam, lParam);
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


void PERelocation::initList(HWND hDlg)
{
	LV_COLUMN lv;
	HWND hListProcessB;
	HWND hListProcessBM;

	memset(&lv, 0, sizeof(LV_COLUMN));
	hListProcessB = GetDlgItem(hDlg, IDC_LIST_PE_RELOCATION_BLOCKS);
	hListProcessBM = GetDlgItem(hDlg, IDC_LIST_PE_RELOCATION_BLOCKSMEMBERS);


	SendMessage(hListProcessB, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	SendMessage(hListProcessBM, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
	lv.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;

	lv.cx = 100;
	lv.pszText = TEXT("Index");
	lv.iSubItem = 0;
	ListView_InsertColumn(hListProcessB, 0, &lv);

	lv.cx = 150;
	lv.pszText = TEXT("Offset");
	lv.iSubItem = 1;
	ListView_InsertColumn(hListProcessB, 1, &lv);

	lv.cx = 200;
	lv.pszText = TEXT("VirtualAddress");
	lv.iSubItem = 2;
	ListView_InsertColumn(hListProcessB, 2, &lv);

	lv.pszText = TEXT("SizeOfBlock");
	lv.iSubItem = 3;
	ListView_InsertColumn(hListProcessB, 3, &lv);

	lv.cx = 100;
	lv.pszText = TEXT("Index");
	lv.iSubItem = 0;
	ListView_InsertColumn(hListProcessBM, 0, &lv);

	lv.cx = 200;
	lv.pszText = TEXT("RVA");
	lv.iSubItem = 1;
	ListView_InsertColumn(hListProcessBM, 1, &lv);

	lv.pszText = TEXT("Offset");
	lv.iSubItem = 2;
	ListView_InsertColumn(hListProcessBM, 2, &lv);
}

void PERelocation::insterList(HWND blockHwnd, int number, TCHAR *numberTC, TCHAR *characterOne, TCHAR *characterTWO, TCHAR *characterThree)
{
	LV_ITEM vitem;
	memset(&vitem, 0, sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;

	vitem.pszText = numberTC;
	vitem.iItem = number;
	vitem.iSubItem = 0;
	ListView_InsertItem(blockHwnd, &vitem);

	vitem.pszText = characterOne;
	vitem.iItem = number;
	vitem.iSubItem = 1;
	ListView_SetItem(blockHwnd, &vitem);

	vitem.pszText = characterTWO;
	vitem.iItem = number;
	vitem.iSubItem = 2;
	ListView_SetItem(blockHwnd, &vitem);

	if (characterThree != NULL) {
		vitem.pszText = characterThree;
		vitem.iItem = number;
		vitem.iSubItem = 3;
		ListView_SetItem(blockHwnd, &vitem);
	}

}

void PERelocation::enumModules(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	HWND blockHwnd = GetDlgItem(hDlg, IDC_LIST_PE_RELOCATION_BLOCKS);
	DWORD dwRowId;
	TCHAR szPid[0x20];
	LV_ITEM lv;

	memset(&lv, 0, sizeof(LV_ITEM));
	memset(szPid, 0, sizeof(TCHAR) * 0x20);

	dwRowId = SendMessage(blockHwnd, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
	if (dwRowId != -1)
	{
		lv.iSubItem = 1;
		lv.pszText = szPid;
		lv.cchTextMax = 0x20;
		SendMessage(blockHwnd, LVM_GETITEMTEXT, dwRowId, (DWORD)&lv);
		int value = 0;
		swscanf_s(szPid, L"%x", &value);

		if (value != 0) {
			HWND membersHwnd = GetDlgItem(hDlg, IDC_LIST_PE_RELOCATION_BLOCKSMEMBERS);
			disBlocksMembers(membersHwnd, value);
		}
	}
}

void PERelocation::disBlocksMembers(HWND membersHwnd, DWORD offset)
{
	ListView_DeleteAllItems(membersHwnd);
	PeToolsClass petc;
	DWORD sizeOfBlock = (petc.getDWValue(pointer + offset + 4, 4) - 8) / 2;
	TCHAR *temValue = NULL;
	temValue = (TCHAR*)malloc(sizeof(TCHAR) * 0x20);
	if (temValue == NULL)
	{
		return;
	}
	for (int i = 0, j = 0; i < sizeOfBlock; i++)
	{
		WORD value = petc.getDWValue(pointer + offset + 8 + (i * 2), 2);
		if ((value >> 12) == 3)
		{
			DWORD RVA = petc.getDWValue(pointer + offset, 4) + (value & 0x0fff);
			DWORD FOA = petc.rvaTofoa(pointer, RVA);

			memset(temValue, 0, sizeof(TCHAR) * 0x20);
			wsprintf(temValue, L"%04d", j + 1);
			wsprintf(temValue + 9, L"%08X", RVA);
			wsprintf(temValue + 18, L"%08X", FOA);

			insterList(membersHwnd, j, temValue, temValue + 9, temValue + 18, NULL);
			j++;
		}
	}
	if (temValue != NULL)
	{
		free(temValue);
	}
}
