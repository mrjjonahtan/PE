#pragma once
class PeToolsClass
{
public:
	PeToolsClass();
	~PeToolsClass();

	void getValue(BYTE *pointerValue, int number, TCHAR *tvlue);
	DWORD getDWValue(BYTE *pointerValue, int number);
};

