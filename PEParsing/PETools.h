#pragma once
#ifndef _PE_PETOOLS_H
#define _PE_PETOOLS_H

#define DOS_POINT 0x200

HINSTANCE PEInstance = NULL;
HWND staticDlg = NULL;
BYTE *pointer = NULL;
PeFile *pef = NULL;
PeOptional *peo = NULL;

void CreatePEDialog(HINSTANCE thisInstance, HWND hDlg);
void selectFile(HWND hDlg);
void PEfun(TCHAR *path);
void freeSpace(TCHAR *point[]);
char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn);
void getValue(BYTE *pointerValue, int number, TCHAR *Tvlue);

INT_PTR CALLBACK DlgProcPEFile(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProcPEDOS(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
#endif