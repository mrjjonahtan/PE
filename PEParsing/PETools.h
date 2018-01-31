#pragma once
#ifndef _PE_PETOOLS_H
#define _PE_PETOOLS_H

#define DOS_POINT 20

HINSTANCE PEInstance = NULL;
HWND staticDlg = NULL;
BYTE *pointer = NULL;

void CreatePEDialog(HINSTANCE thisInstance, HWND hDlg);
void selectFile(HWND hDlg);
void PEfun(TCHAR *path);
void freeSpace(TCHAR *point[]);
char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn);
TCHAR* getValue(BYTE *pointerValue, int number);

BOOL CALLBACK DlgProcPEFile(HWND hDlg, UINT iMessage, UINT wParam, LONG lParam);
BOOL CALLBACK DlgProcPEDOS(HWND hDlg, UINT iMessage, UINT wParam, LONG lParam);
#endif