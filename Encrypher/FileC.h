#pragma once
#include <conio.h>
#include <stdio.h>
#include <direct.h>
#include <fstream>
#include <iterator> 
#include <sys/stat.h>
#include <string>
#include <windows.h>
using namespace std;
class File {
public:
	static bool Exists(const string& name) {
		DWORD dwAttrib = GetFileAttributesA(name.c_str());
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}
	static string getexepath()
	{
		char tuh[_MAX_PATH];
		char *abc;
		abc = _getcwd(tuh, sizeof(tuh));
		return abc;
	}
	static long FileSize(string Path) {
		struct stat stat_buf;
		int rc = stat(Path.c_str(), &stat_buf);
		return rc == 0 ? stat_buf.st_size : -1;
	}
	static std::string RemovePath(const std::string Path) {
		string c = Path;
		const size_t last_slash_idx = c.find_last_of("\\/");
		if (string::npos != last_slash_idx)
		{
			c.erase(0, last_slash_idx + 1);
		}
		return c;
	}
};