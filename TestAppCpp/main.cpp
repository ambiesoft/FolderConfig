// TestAppCpp.cpp : Defines the entry point for the console application.
//



#include <Windows.h>

#include "../../profile/cpp/Profile/include/ambiesoft.profile.h"

using namespace Ambiesoft;
using namespace std;

#include <locale>
#include <codecvt>
#include <string>
#include <sstream>

// https://stackoverflow.com/a/18597384
wstring toWString(const string& s)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	// std::string narrow = converter.to_bytes(wide_utf16_source_string);
	std::wstring wide = converter.from_bytes(s);
	return wide;
}

int __stdcall wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR     lpCmdLine,
	int       nShowCmd
	)
{
	WCHAR szInifile[MAX_PATH];
	GetModuleFileName(NULL, szInifile, _countof(szInifile));

	// cut filname
	*(wcsrchr(szInifile, L'\\') + 1) = 0;

	// cat ini
	wcscat_s(szInifile, L"folder.ini");

	wstringstream ss;

	int pathType = -1;
	Profile::GetInt("Main", "PathType", 0, pathType, szInifile);

	ss << L"PathType=" << pathType << endl;

	if (pathType == 3)
	{
		// user specified path
		string folder;
		if (!Profile::GetString("Main", "folder", "", folder, szInifile))
		{
			MessageBox(nullptr, L"Error", nullptr, MB_ICONERROR);
			return 1;
		}

		ss << L"folder=" << toWString(folder) << endl;
	}

	MessageBox(nullptr, ss.str().c_str(), nullptr, MB_ICONINFORMATION);
	return 0;
}

