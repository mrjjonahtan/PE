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
			temporaryBUffer = (TCHAR*)malloc(sizeof(TCHAR) * 0x200);
			if (temporaryBUffer == NULL)
			{
				return false;
			}
			memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x200);

			DWORD directoryLocat = petc.getDWValue((per->pointer + 60), 4) + 24 + 96;
			//DWORD RelocationRVA = petc.getDWValue((per->pointer + directoryLocat + (10 * 4)), 4);
			DWORD RelocationFOA = petc.rvaTofoa(per->pointer, petc.getDWValue((per->pointer + directoryLocat + (10 * 4)), 4));




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

	lv.cx = 200;
	lv.pszText = TEXT("VirtualAddress");										
	lv.iSubItem = 1;					
	ListView_InsertColumn(hListProcessB, 1, &lv);

	lv.pszText = TEXT("SizeOfBlock");										
	lv.iSubItem = 2;					
	ListView_InsertColumn(hListProcessB, 2, &lv);

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

void PERelocation::insterList(HWND hDlg, int number, TCHAR *numberTC, TCHAR *characterOne, TCHAR *characterTWO)
{
	LV_ITEM vitem;
	HWND hListProcess = NULL;
	hListProcess = GetDlgItem(hDlg, IDC_LIST_PE_EXPORT);

	memset(&vitem, 0, sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;

	vitem.pszText = numberTC;
	vitem.iItem = number;
	vitem.iSubItem = 0;
	ListView_InsertItem(hListProcess, &vitem);

	vitem.pszText = characterOne;
	vitem.iItem = number;
	vitem.iSubItem = 1;
	ListView_SetItem(hListProcess, &vitem);

	vitem.pszText = characterTWO;
	vitem.iItem = number;
	vitem.iSubItem = 2;
	ListView_SetItem(hListProcess, &vitem);
}
