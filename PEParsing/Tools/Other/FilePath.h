#pragma once
class FilePath
{
public:
	FilePath();
	~FilePath();
	void getRootDirectory(wchar_t *rootDirectory, size_t numberOfElements);
};

