#pragma once
class PEEncryptedshell
{
private:
	HINSTANCE peInstance = NULL;

public:
	PEEncryptedshell();
	~PEEncryptedshell();

private :
	void encryptionAlgorithm();
	void decryptionAlgorithm();
	void addData(HWND hDlg);
public:
	void createDialog(HINSTANCE PeInstance);
	void selectFile(HWND hDlg ,BYTE style);

	friend INT_PTR CALLBACK encryptedShellDialog(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

