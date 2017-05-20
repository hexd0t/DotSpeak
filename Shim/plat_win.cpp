#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "plat.h"
#include <cstdio>
#include <cstdlib>
#include <Shlwapi.h>

const size_t FILENAME_BUFSIZE = 2048u;

HINSTANCE WIN_MODULE_HANDLE = nullptr;
std::string FILENAME;
std::string PLUGINDIR;
WCHAR* FILENAME_UTF16 = nullptr;
WCHAR* PATH_UTF16 = nullptr;
std::string FILENAME_EXTLESS;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		WIN_MODULE_HANDLE = hinstDLL;
		break;
	}
	return TRUE;
}

void calculatePathsUTF16() {
	WCHAR buf[FILENAME_BUFSIZE];
	GetModuleFileNameW(WIN_MODULE_HANDLE, buf, FILENAME_BUFSIZE);

	LPWSTR filename = PathFindFileNameW(buf);
	size_t filename_len = wcslen(filename) + 1;
	size_t path_len = filename - buf + 1;

	FILENAME_UTF16 = new WCHAR[filename_len];
	PATH_UTF16 = new WCHAR[path_len];
	wcscpy_s(FILENAME_UTF16, filename_len, filename);
	wcsncpy_s(PATH_UTF16, path_len, buf, path_len - 1);

	PATH_UTF16[path_len - 1] = 0;
}

WCHAR* GetFilenameUTF16() {
	if (!FILENAME_UTF16)
		calculatePathsUTF16();
	return FILENAME_UTF16;
}

WCHAR* GetPathUTF16() {
	if (!PATH_UTF16)
		calculatePathsUTF16();
	return PATH_UTF16;
}

const std::string& GetFilename() {
	if (!FILENAME.empty())
		return FILENAME;

	GetFilenameUTF16();

	int outlen = WideCharToMultiByte(CP_UTF8, 0, FILENAME_UTF16, -1, 0, 0, 0, 0);
	char* buf = new char[outlen];
	WideCharToMultiByte(CP_UTF8, 0, FILENAME_UTF16, -1, buf, outlen, 0, 0);

	FILENAME = std::string(buf);

	return FILENAME;
}

const std::string& GetPluginsDir() {
	if (!PLUGINDIR.empty())
		return PLUGINDIR;

	GetPathUTF16();

	int outlen = WideCharToMultiByte(CP_UTF8, 0, PATH_UTF16, -1, 0, 0, 0, 0);
	char* buf = new char[outlen];
	WideCharToMultiByte(CP_UTF8, 0, PATH_UTF16, -1, buf, outlen, 0, 0);

	PLUGINDIR = std::string(buf);

	return PLUGINDIR;
}

const std::string& GetFilenameWithoutExt() {
	if (!FILENAME_EXTLESS.empty())
		return FILENAME_EXTLESS;
	GetFilename();
	GetPluginsDir();

	std::size_t found = FILENAME.find_last_of(".");

	if (found == std::string::npos)
		found = FILENAME.length();

	FILENAME_EXTLESS = FILENAME.substr(0, found);
	return FILENAME_EXTLESS;
}

void errmsg(const std::string& msg) {
	MessageBoxA(0, msg.c_str(), "DotSpeak error", 0);
}
