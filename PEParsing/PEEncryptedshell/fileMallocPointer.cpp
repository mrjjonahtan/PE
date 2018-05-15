#include "..\stdafx.h"
#include "fileMallocPointer.h"


fileMallocPointer::fileMallocPointer()
{
}


fileMallocPointer::~fileMallocPointer()
{
}

//malloc
DWORD fileMallocPointer::filemalloc(char *path, BYTE **pointer,DWORD addSize)
{
	FILE * shellFilePointer = NULL;
	unsigned int shellFileSize = 0;

	fopen_s(&shellFilePointer, path, "rb");
	if (shellFilePointer == NULL) {
		return 0;
	}
	if (fseek(shellFilePointer, 0L, SEEK_END)) {
		return 0;
	}
	shellFileSize = ftell(shellFilePointer);
	if (shellFileSize == 0) {
		return 0;
	}

	shellFileSize = shellFileSize + addSize;

	*pointer = (BYTE*)malloc(shellFileSize);
	if (*pointer == NULL) {
		return 0;
	}
	memset(*pointer, 0L, sizeof(BYTE)*shellFileSize);

	fseek(shellFilePointer, 0L, 0L);
	if (fread(*pointer, shellFileSize - addSize, 1, shellFilePointer) <= 0) {
		return 0;
	}

	fclose(shellFilePointer);
	shellFilePointer = NULL;

	return shellFileSize;
}
