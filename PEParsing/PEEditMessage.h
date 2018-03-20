#pragma once
class PEEditMessage
{
private:
	HWND edtHwnd = NULL;
public:
	PEEditMessage();
	~PEEditMessage();
public:
	void setMessageText(HWND hDlg, BYTE *pointer);
};

