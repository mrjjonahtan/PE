#pragma once
class PEEditMessage
{
public:
	PEEditMessage();
	~PEEditMessage();
public:
	void setMessageText(HWND hDlg, BYTE *pointer,DWORD fileSize);

};

