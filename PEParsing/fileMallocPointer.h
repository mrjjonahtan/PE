#pragma once
class fileMallocPointer
{
private:
	
public:
	fileMallocPointer();
	~fileMallocPointer();
	DWORD filemalloc(char *path,BYTE **pointer, DWORD addSize);
};

