#pragma once
#ifndef _PE_PETOOLS_H
#define _PE_PETOOLS_H

#define DOS_POINT 0x200

HINSTANCE PEInstance = NULL;
HWND staticDlg = NULL;
HWND editDlg = NULL;
BYTE *pointer = NULL;
PeFile *pef = NULL;
PeOptional *peo = NULL;
PeDirectory *ped = NULL;
PESection *pes = NULL;
PEExport *pee = NULL;
PERelocation *per = NULL;
PEImport *pei = NULL;
PEBoundImport *pebi = NULL;
PEEditMessage *peem = NULL;

void CreatePEDialog(HINSTANCE thisInstance, HWND hDlg);
void selectFile(HWND hDlg);
void PEfun(wchar_t *path);
void freeSpace(wchar_t *point[]);
char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn);

INT_PTR CALLBACK DlgProcPEFile(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProcPEDOS(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
#endif