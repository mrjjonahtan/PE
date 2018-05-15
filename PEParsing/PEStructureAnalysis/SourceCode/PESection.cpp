#include "..\..\stdafx.h"
#include "..\index\PESection.h"
#include "..\..\Tools\PE\PeToolsClass.h"


PESection::PESection()
{
}


PESection::~PESection()
{
}

void PESection::creadPESectionDialog(HINSTANCE PeInstance, BYTE *fpointer)
{
	peInstance = PeInstance;
	pointer = fpointer;
	if (peInstance != NULL)
	{
		DialogBox(peInstance, MAKEINTRESOURCE(IDD_DIALOG_PE_SECTION), NULL, (DLGPROC)PeSectionDialog);
	}
}

INT_PTR CALLBACK PeSectionDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	extern PESection *pes;
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
		pes->initList(hDlg);
		if (pes->pointer != NULL)
		{
			PeToolsClass petc;
			TCHAR *temporaryBUffer = NULL;
			TCHAR *numer = NULL;
			temporaryBUffer = (TCHAR*)malloc(sizeof(TCHAR) * 0x80);
			if (temporaryBUffer == NULL)
			{
				return false;
			}
			DWORD pelocat = petc.getPELocation(pes->pointer);
			DWORD optionSize = petc.getOptionSizeValue(pes->pointer);
			int snumber = petc.getSectionNumber(pes->pointer);
			numer = (TCHAR *)malloc(sizeof(TCHAR) * 0x10);
			if (numer == NULL)
			{
				return false;
			}
			memset(numer, 0, sizeof(TCHAR) * 0x10);
			wsprintf(numer, L"%d", snumber);
			SetWindowText(GetDlgItem(hDlg, IDC_STATIC_PE_SECTION_NUMBER), numer);
			for (int i = 0; i < snumber; i++)
			{
				DWORD locat = i * 40;
				memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x80);
				petc.getCharPointer((pes->pointer + pelocat + optionSize + 24 + locat), temporaryBUffer, 8);
				petc.getValue((pes->pointer + pelocat + optionSize + 24 + 8 + locat), 4, (temporaryBUffer + 9 ));
				petc.getValue((pes->pointer + pelocat + optionSize + 24 + 12 + locat), 4, (temporaryBUffer + 18 ));
				petc.getValue((pes->pointer + pelocat + optionSize + 24 + 16 + locat), 4, (temporaryBUffer + 27 ));
				petc.getValue((pes->pointer + pelocat + optionSize + 24 + 20 + locat), 4, (temporaryBUffer + 36 ));
				petc.getValue((pes->pointer + pelocat + optionSize + 24 + 36 + locat), 4, (temporaryBUffer + 45 ));
				pes->insertList(hDlg, i, temporaryBUffer , (temporaryBUffer + 9 ), (temporaryBUffer + 18 ), (temporaryBUffer + 27 ), (temporaryBUffer + 36 ), (temporaryBUffer + 45 ));
			}
			if (temporaryBUffer != NULL)
			{
				free(temporaryBUffer);
				temporaryBUffer = NULL;
			}
			if (numer != NULL)
			{
				free(numer);
				numer = NULL;
			}
		}
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BTN_SECTION_CANCEL:
		{
			EndDialog(hDlg, 0);
		}
		break;
		case IDC_BTN_SECTION_REFRESH:
		{

		}
		break;
		case IDC_BTN_SECTION_SAVE:
		{

		}
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

void PESection::initList(HWND hDlg)
{
	LV_COLUMN lv;
	HWND hListProcess = NULL;

	//初始化
	memset(&lv, 0, sizeof(LV_COLUMN));
	hListProcess = GetDlgItem(hDlg, IDC_LIST_SECTION_TAB);

	//设置选中行
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
	lv.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;
	lv.cx = 113;

	lv.pszText = TEXT("名称");
	lv.iSubItem = 0;
	ListView_InsertColumn(hListProcess, 0, &lv);

	lv.pszText = TEXT("内存中大小(V)");
	lv.iSubItem = 1;
	ListView_InsertColumn(hListProcess, 1, &lv);

	lv.pszText = TEXT("内存中偏移(V)");
	lv.iSubItem = 2;
	ListView_InsertColumn(hListProcess, 2, &lv);

	lv.pszText = TEXT("文件中大小(R)");
	lv.iSubItem = 3;
	ListView_InsertColumn(hListProcess, 3, &lv);

	lv.pszText = TEXT("文件中偏移(R)");
	lv.iSubItem = 4;
	ListView_InsertColumn(hListProcess, 4, &lv);

	lv.pszText = TEXT("标志");
	lv.cx = 115;
	lv.iSubItem = 5;
	ListView_InsertColumn(hListProcess, 5, &lv);
}

void PESection::insertList(HWND hDlg, int number, TCHAR *name, TCHAR *memory, TCHAR *memorySize, TCHAR *file, TCHAR *fileSize, TCHAR *flag)
{
	LV_ITEM vitem;
	HWND hListProcess = NULL;
	hListProcess = GetDlgItem(hDlg, IDC_LIST_SECTION_TAB);

	memset(&vitem, 0, sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;

	vitem.pszText = name;
	vitem.iItem = number;
	vitem.iSubItem = 0;
	ListView_InsertItem(hListProcess, &vitem);

	vitem.pszText = memory;
	vitem.iItem = number;
	vitem.iSubItem = 1;
	ListView_SetItem(hListProcess, &vitem);

	vitem.pszText = memorySize;
	vitem.iItem = number;
	vitem.iSubItem = 2;
	ListView_SetItem(hListProcess, &vitem);

	vitem.pszText = file;
	vitem.iItem = number;
	vitem.iSubItem = 3;
	ListView_SetItem(hListProcess, &vitem);

	vitem.pszText = fileSize;
	vitem.iItem = number;
	vitem.iSubItem = 4;
	ListView_SetItem(hListProcess, &vitem);

	vitem.pszText = flag;
	vitem.iItem = number;
	vitem.iSubItem = 5;
	ListView_SetItem(hListProcess, &vitem);
}
