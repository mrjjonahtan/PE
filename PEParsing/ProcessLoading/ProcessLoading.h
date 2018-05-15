#pragma once
class ProcessLoading
{
public:
	HANDLE hProcess;
	unsigned long imageBase;
	unsigned char *pebuffer;
	unsigned long messageBufferSize;
	unsigned long addressOfEntryPoint;
	unsigned long directoryLocat;
	unsigned long importLocat;

public:
	ProcessLoading();
	~ProcessLoading();
	int processLoading(char *path);//main

private:
	void init();
	void repairIAT(BYTE *pointer);
	void importDescriptor(BYTE *pointer);
	void getMessage(BYTE *srcPointer);
	void getDate(char *path);
};

