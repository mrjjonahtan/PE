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
		pee->initList(hDlg);
		if (pee->editHwnd[0] == NULL)
		{
			for (int i = 0; i < 10; i++)
			{
				pee->editHwnd[i] = GetDlgItem(hDlg, pee->editID[i]);
			}
		}
		if (pee->pointer == NULL)
		{
			for (int i = 0; i < 10; i++)
			{
				SetWindowText(pee->editHwnd[i], L"00000000");
			}
		}
		else
		{
			PeToolsClass petc;
			TCHAR *temporaryBUffer = NULL;
			temporaryBUffer = (TCHAR*)malloc(sizeof(TCHAR) * 0x40);
			if (temporaryBUffer == NULL)
			{
				return false;
			}
			memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x40);

			DWORD platform = petc.getApplicationSize(pee->pointer);
			DWORD directoryLocat = 0;
			if (platform == 0x014C)
			{
				directoryLocat = petc.getDWValue((pee->pointer + 60), 4) + 24 + 96;
			}
			else if (platform == 0x8664)
			{
				directoryLocat = petc.getDWValue((pee->pointer + 60), 4) + 24 + 112;
			}

			DWORD exportRVA = petc.getDWValue((pee->pointer + directoryLocat), 4);
			if (exportRVA == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					SetWindowText(pee->editHwnd[i], L"00000000");
				}
				break;
			}
			DWORD exportFOA = petc.rvaTofoa((pee->pointer), exportRVA);
			wsprintf(temporaryBUffer, L"%02X", exportFOA);
			SetWindowText(pee->editHwnd[0], temporaryBUffer);
			petc.getValue((pee->pointer + exportFOA), 4, temporaryBUffer + 9);
			petc.getValue((pee->pointer + exportFOA + 12), 4, temporaryBUffer + 18);//name
			//DWORD nameRVA = petc.getDWValue((pee->pointer + exportFOA + 12), 4);
			DWORD nameFOA = petc.rvaTofoa(pee->pointer, petc.getDWValue((pee->pointer + exportFOA + 12), 4));
			petc.getValue((pee->pointer + exportFOA + 16), 4, temporaryBUffer + 27);//base
			petc.getCharPointer((pee->pointer + nameFOA), temporaryBUffer + 36, 0);//name string
			SetWindowText(pee->editHwnd[1], temporaryBUffer + 9);
			SetWindowText(pee->editHwnd[2], temporaryBUffer + 27);
			SetWindowText(pee->editHwnd[3], temporaryBUffer + 18);
			SetWindowText(pee->editHwnd[4], temporaryBUffer + 36);

			memset(temporaryBUffer, 0, sizeof(TCHAR) * 0x40);
			petc.getValue((pee->pointer + exportFOA + 20), 4, temporaryBUffer);
			petc.getValue((pee->pointer + exportFOA + 24), 4, temporaryBUffer + 9);
			petc.getValue((pee->pointer + exportFOA + 28), 4, temporaryBUffer + 18);
			petc.getValue((pee->pointer + exportFOA + 32), 4, temporaryBUffer + 27);
			petc.getValue((pee->pointer + exportFOA + 36), 4, temporaryBUffer + 36);
			SetWindowText(pee->editHwnd[5], temporaryBUffer);
			SetWindowText(pee->editHwnd[6], temporaryBUffer + 9);
			SetWindowText(pee->editHwnd[7], temporaryBUffer + 18);
			SetWindowText(pee->editHwnd[8], temporaryBUffer + 27);
			SetWindowText(pee->editHwnd[9], temporaryBUffer + 36);

			if (temporaryBUffer != NULL)
			{
				free(temporaryBUffer);
				temporaryBUffer = NULL;
			}

			DWORD funNumbers = petc.getDWValue((pee->pointer + exportFOA + 20), 4);
			DWORD nameFunNumbers = petc.getDWValue((pee->pointer + exportFOA + 24), 4);
			//DWORD addressFunRVA = petc.getDWValue((pee->pointer + exportFOA + 28), 4);
			//DWORD addressNameRVA = petc.getDWValue((pee->pointer + exportFOA + 32), 4);
			//DWORD addressOrdinRVA = petc.getDWValue((pee->pointer + exportFOA + 36), 4);
			DWORD addressFunFOA = petc.rvaTofoa(pee->pointer, petc.getDWValue((pee->pointer + exportFOA + 28), 4));
			DWORD addressNameFOA = petc.rvaTofoa(pee->pointer, petc.getDWValue((pee->pointer + exportFOA + 32), 4));
			DWORD addressOrdinFOA = petc.rvaTofoa(pee->pointer, petc.getDWValue((pee->pointer + exportFOA + 36), 4));

			if (funNumbers != 0) {
				DWORD *funNum = NULL;
				DWORD *name = NULL;
				WORD *ordin = NULL;
				funNum = (DWORD*)malloc(sizeof(DWORD)*funNumbers);
				name = (DWORD*)malloc(sizeof(DWORD)*nameFunNumbers);
				ordin = (WORD*)malloc(sizeof(WORD)*nameFunNumbers);
				if (funNum == NULL || ordin == NULL || name == NULL) {
					break;
				}
				memset(funNum, 0, sizeof(DWORD)*funNumbers);
				memset(name, 0, sizeof(DWORD)*nameFunNumbers);
				memset(ordin, 0, sizeof(WORD)*nameFunNumbers);
				//fun
				for (int i = 0; i < funNumbers; i++)
				{
					*(funNum + i) = petc.getDWValue((pee->pointer + addressFunFOA + (i * 4)), 4);
				}
				//ordin
				for (int i = 0; i < nameFunNumbers; i++)
				{
					*(ordin + i) = petc.getDWValue((pee->pointer + addressOrdinFOA + (i * 2)), 2);
				}
				//name
				for (int i = 0; i < nameFunNumbers; i++)
				{
					*(name + i) = petc.getDWValue((pee->pointer + addressNameFOA + (i * 4)), 4);
				}
				TCHAR *temTC = NULL;
				temTC = (TCHAR*)malloc(sizeof(TCHAR) * 0x60);
				if (temTC == NULL)
				{
					break;
				}
				for (int i = 0; i < nameFunNumbers; i++)
				{
					memset(temTC, 0, sizeof(TCHAR) * 0x60);
					//ordin
					DWORD ordintem = *(ordin + i);
					wsprintf(temTC, L"%04X", ordintem);
					//funRVA
					DWORD funRVA = *(funNum + ordintem);
					wsprintf(temTC + 9, L"%08X", funRVA);
					//funFOA
					DWORD funFOA = petc.rvaTofoa(pee->pointer, funRVA);
					wsprintf(temTC + 18, L"%08X", funFOA);
					//name
					//DWORD nameRVA = *(name + i);
					DWORD nameFOA = petc.rvaTofoa(pee->pointer, *(name + i));
					petc.getCharPointer((pee->pointer + nameFOA), (temTC + 27), 0);

					pee->insertList(hDlg, i, temTC, temTC + 9, temTC + 18, temTC + 27);
				}
				if (temTC != NULL) {
					free(temTC);
				}

				if (funNum != NULL && ordin != NULL && name != NULL) {
					free(funNum);
					free(name);
					free(ordin);
				}

			}

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