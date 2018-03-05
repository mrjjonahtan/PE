#include "stdafx.h"
#include "PeToolsClass.h"


PeToolsClass::PeToolsClass()
{
}


PeToolsClass::~PeToolsClass()
{
}

void PeToolsClass::getValue(BYTE *pointerValue, int number, TCHAR *tvlue)
{
	char *vaby = NULL;
	vaby = (char*)malloc(0x200);
	if (vaby == NULL)
	{
		return;
	}
	for (int i = number - 1, j = 0; i >= 0; i--, j++)
	{
		memset(vaby, 0, 0x200);
		DWORD valueTem = *(pointerValue + i);
		sprintf_s(vaby, 0x200, "%02X", valueTem);
		for (int k = 0; *(vaby + k) != 0; k++)
		{
			*(tvlue + j) = *(vaby + k);
			j++;
		}
		j--;
	}
	if (vaby != NULL) {
		free(vaby);
	}
}

DWORD PeToolsClass::getDWValue(BYTE *pointerValue, int number)
{
  	DWORD revalue = 0;
	for (int i = (number-1); i >= 0; i--)
	{
		revalue = (revalue << 8) + *(pointerValue + i);
	}
	return revalue;
}
