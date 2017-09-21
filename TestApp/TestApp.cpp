// TestApp.cpp : main project file.

#include "stdafx.h"

#include "../../lsMisc/stdwin32/stdwin32.h"
#include "../../lsMisc/UTF16toUTF8.h"

using namespace System;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Diagnostics;

using namespace Ambiesoft;
using namespace stdwin32;

using std::wstring;

int main(array<System::String ^> ^args)
{
	try
	{
		String^ path = FolderConfigHelper::GetConfigPath();

		Console::WriteLine(String::Format("path is \"{0}\".",path));
		Console::WriteLine(String::Format("Path is acceesable...? {0}", FolderConfigHelper::IsFolderAccessible(path)));


		// try 2nd path
		String^ folconexe = Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location);
		folconexe = Path::Combine(folconexe, "FolderConfig.exe");
		String^ cmdline = String::Format("/section {0} /defaultpathtype {1} /creator Ambiesoft /appname MyApp",
			"Ninja",
			2);
		
		Process^ pro = Process::Start(folconexe,cmdline);
		pro->WaitForExit();
		
		if(SUCCEEDED(pro->ExitCode))
		{
			Console::Write("Succeeded ");
			if(pro->ExitCode & 1)
				Console::WriteLine("OK");
			else
				Console::WriteLine("Cancel");
		}
		else
		{
			Console::Write("Failed ");
			if(pro->ExitCode & 1)
				Console::WriteLine("OK");
			else
				Console::WriteLine("Cancel");
		}

		String^ ninjaPath = FolderConfigHelper::GetConfigPath("Ninja");
		Console::WriteLine("Ninja path={0}", ninjaPath);

		wstring inipath = stdCombinePath(stdGetParentDirectory(stdGetModuleFileName()), L"folder.ini");
		int pathtype = GetPrivateProfileInt(L"Ninja", L"PathType", 0, inipath.c_str());
		Console::WriteLine("Win32PathType={0}", pathtype);
		WCHAR szTw[MAX_PATH];
		GetPrivateProfileString(L"Ninja", L"Folder", NULL, szTw, _countof(szTw), inipath.c_str());
		Console::WriteLine("Win32Path={0}", gcnew String(szTw));

		char szTa[MAX_PATH];
		GetPrivateProfileStringA("Ninja", "Folder", NULL, szTa, _countof(szTw), "C:\\Linkout\\FolderConfigD\\folder.ini");
		Console::WriteLine("Win32APathA={0}", gcnew String(szTa));
		wstring ws;
		UTF8toUTF16((LPBYTE)szTa,ws);
		Console::WriteLine("Win32APathAUTF8toUTF16={0}", gcnew String(ws.c_str()));
	}
	catch(Exception^ ex)
	{
		Console::Error->WriteLine(ex->Message);
	}
	
	return 0;
}
