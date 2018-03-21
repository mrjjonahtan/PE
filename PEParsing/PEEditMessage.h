#pragma once
class PEEditMessage
{
private:
	HWND edtHwnd = NULL;
	wchar_t *message = NULL;
	wchar_t *temChar = NULL;
public:
	PEEditMessage();
	~PEEditMessage();
public:
	void setMessageText(HWND hDlg, BYTE *pointer);
private:
	void additionalCharts(wchar_t *messageT);
};

