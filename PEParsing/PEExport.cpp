#include "stdafx.h"
#include "PEExport.h"
#include "PeToolsClass.h"

PEExport::PEExport()
{
}


PEExport::~PEExport()
{
}

void PEExport::creadPEExportDialog(HINSTANCE PeInstance, BYTE *fpointer)
{
	peInstance = PeInstance;
	pointer = fpointer;
	if (peInstance != NULL)
	{
		DialogBox(peInstance, MAKEINTRESOURCE(IDD_DIALOG_PE_EXPORT), NULL, (DLGPROC)peExportDialog);
	}
}

INT_PTR CALLBACK peExportDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	extern PEExport *pee;
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
		PeToolsClass petc;
		pee->initList(hDlg);
		if (pee->editHwnd[0] == NULL)
		{
			for (int i = 0; i < 10; i++)
			{
				pee->editHwnd[i] = GetDlgItem(hDlg, pee->editID[i]);
			}
		}
		TCHAR *temporaryBUffer = NULL;
		temporaryBUffer = (TCHAR*)malloc(sizeof(TCHAR) * 0x100);
		if (temporaryBUffer == NULL)
		{
			return false;
		}
		memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x100);
		DWORD directoryLocat = petc.getDWValue((pee->pointer + 60), 4) + 24 + 96;
		DWORD exportRVA = petc.getDWValue((pee->pointer + directoryLocat), 4);
		DWORD exportFOA = petc.rvaTofoa((pee->pointer), exportRVA);

		if (temporaryBUffer != NULL)
		{
			free(temporaryBUffer);
			temporaryBUffer = NULL;
		}
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BTN_PE_EXPORT_CANTEL:
			EndDialog(hDlg, 0);
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

void PEExport::initList(HWND hDlg)
{
	LV_COLUMN lv;
	HWND hListProcess = NULL;

	//初始化
	memset(&lv, 0, sizeof(LV_COLUMN));
	hListProcess = GetDlgItem(hDlg, IDC_LIST_PE_EXPORT);

	//设置选中行
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
	lv.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;
	lv.cx = 160;

	lv.pszText = TEXT("Ordinals");
	lv.iSubItem = 0;
	ListView_InsertColumn(hListProcess, 0, &lv);

	lv.pszText = TEXT("RVA");
	lv.iSubItem = 1;
	ListView_InsertColumn(hListProcess, 1, &lv);

	lv.pszText = TEXT("Offset");
	lv.iSubItem = 2;
	ListView_InsertColumn(hListProcess, 2, &lv);

	lv.pszText = TEXT("Function Name");
	lv.cx = 268;
	lv.iSubItem = 3;
	ListView_InsertColumn(hListProcess, 3, &lv);
}

void PEExport::insertList(HWND hDlg, int number, TCHAR *ordinals, TCHAR *rva, TCHAR *offset, TCHAR *name)
{
	LV_ITEM vitem;
	HWND hListProcess = NULL;
	hListProcess = GetDlgItem(hDlg, IDC_LIST_PE_EXPORT);

	memset(&vitem, 0, sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;

	vitem.pszText = ordinals;
	vitem.iItem = number;
	vitem.iSubItem = 0;
	ListView_InsertItem(hListProcess, &vitem);

	vitem.pszText = rva;
	vitem.iItem = number;
	vitem.iSubItem = 1;
	ListView_SetItem(hListProcess, &vitem);

	vitem.pszText = offset;
	vitem.iItem = number;
	vitem.iSubItem = 2;
	ListView_SetItem(hListProcess, &vitem);

	vitem.pszText = name;
	vitem.iItem = number;
	vitem.iSubItem = 3;
	ListView_SetItem(hListProcess, &vitem);
}