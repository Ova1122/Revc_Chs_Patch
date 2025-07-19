#pragma once

typedef  int(F_CFileMgr_OpenFile)(const char* file, const char* mode);


typedef  int(F_CFileMgr_CloseFile)(int);


class CCFileMgr
{
public:
	static F_CFileMgr_OpenFile* dOpenFile;
	static F_CFileMgr_CloseFile* dCloseFile;

	static int OpenFile(const char* file, const char* mode)
	{
		return dOpenFile(file, mode);
	}
	static int CloseFile(int fd)
	{
		return dCloseFile(fd);
	}
};