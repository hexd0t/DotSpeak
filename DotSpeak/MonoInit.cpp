#include <string>
#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>

#ifdef _WIN32 //------------------------------------------------------ WIN
#define EXPORTDLL __declspec(dllexport)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Shlwapi.h>

const size_t FILENAME_BUFSIZE = 2048u;
HINSTANCE WIN_MODULE_HANDLE = nullptr;

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

std::string GetInstallDir() {
	WCHAR buf[FILENAME_BUFSIZE];
	GetModuleFileNameW(WIN_MODULE_HANDLE, buf, FILENAME_BUFSIZE);

	LPWSTR filename = PathFindFileNameW(buf);
	size_t path_len = filename - buf + 1;
	auto path_utf16 = new WCHAR[path_len];
	wcsncpy_s(path_utf16, path_len, buf, path_len - 1);

	path_utf16[path_len - 1] = 0;

	int outlen = WideCharToMultiByte(CP_UTF8, 0, path_utf16, -1, 0, 0, 0, 0);
	char* nbuf = new char[outlen];
	WideCharToMultiByte(CP_UTF8, 0, path_utf16, -1, nbuf, outlen, 0, 0);

	return std::string(nbuf);
}

#else //------------------------------------------------------ OTHER: TBD
#define EXPORTDLL __attribute__ ((visibility("default")))
#endif

static bool MONOINIT = false;
static MonoDomain* MONODOMAIN = nullptr;

extern "C" {
	EXPORTDLL void InitMono() {
		if (MONOINIT)
			return;

		auto monolib = GetInstallDir() + "/DotSpeak/mono/lib";
		auto monoetc = GetInstallDir() + "/DotSpeak/mono/etc";
		mono_set_dirs(monolib.c_str(), monoetc.c_str());

		MONODOMAIN = mono_jit_init_version("DotSpeak", "v4.0.30319");

		MONOINIT = true;
	}
}