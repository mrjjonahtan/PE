#include "..\..\stdafx.h"
#include "FilePath.h"

#include <direct.h>

FilePath::FilePath()
{
}


FilePath::~FilePath()
{
}

void FilePath::getRootDirectory(wchar_t *rootDirectory, size_t numberOfElements)
{
	GetModuleFileName(NULL, (LPWSTR)rootDirectory, numberOfElements);

	/*wchar_t *name = wcsrchr(rootDirectory, L'\\');

	wcsncpy_s(rootDirectory, numberOfElements,rootDirectory, (wcslen(rootDirectory) - wcslen(name) + 1));*/

	//Debug
	wchar_t *name = wcsrchr(rootDirectory, L'D');

	wcsncpy_s(rootDirectory, numberOfElements, rootDirectory, (wcslen(rootDirectory) - wcslen(name))); 
}
