#include "stdafx.h"
#include "PESection.h"
#include "PeToolsClass.h"


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

	//��ʼ��
	memset(&lv, 0, sizeof(LV_COLUMN));
	hListProcess = GetDlgItem(hDlg, IDC_LIST_SECTION_TAB);

	//����ѡ����
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
	lv.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;
	lv.cx = 113;

	lv.pszText = TEXT("����");										
	lv.iSubItem = 0;					
	ListView_InsertColumn(hListProcess, 0, &lv);

	lv.pszText = TEXT("�ڴ���ƫ��");	
	lv.iSubItem = 1;					
	ListView_InsertColumn(hListProcess, 1, &lv);

	lv.pszText = TEXT("�ڴ��д�С");		
	lv.iSubItem = 2;					
	ListView_InsertColumn(hListProcess, 2, &lv);

	lv.pszText = TEXT("�ļ���ƫ��");		
	lv.iSubItem = 3;					
	ListView_InsertColumn(hListProcess, 3, &lv);

	lv.pszText = TEXT("�ļ��д�С");		
	lv.iSubItem = 4;					
	ListView_InsertColumn(hListProcess, 4, &lv);

	lv.pszText = TEXT("��־");	
	lv.cx = 115;
	lv.iSubItem = 5;					
	ListView_InsertColumn(hListProcess, 5, &lv);

}
