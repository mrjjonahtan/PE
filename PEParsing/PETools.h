#pragma once
#ifndef _PE_PETOOLS_H
#define _PE_PETOOLS_H

#define DOS_POINT 0x200

HINSTANCE PEInstance = NULL;
HWND staticDlg = NULL;
BYTE *pointer = NULL;

void CreatePEDialog(HINSTANCE thisInstance, HWND hDlg);
void selectFile(HWND hDlg);
void PEfun(TCHAR *path);
void freeSpace(TCHAR *point[]);
char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn);
void getValue(BYTE *pointerValue, int number, TCHAR *Tvlue);

INT_PTR CALLBACK DlgProcPEFile(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProcPEDOS(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
<<<<<<< HEAD
=======

>>>>>>> 61b06467431b9968a9f6babd0f5164b30c64732f
#endif