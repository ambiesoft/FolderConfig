// TestApp.cpp : main project file.

#include "stdafx.h"
#include <Winerror.h>

using namespace System;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Diagnostics;

using namespace Ambiesoft;

int main(array<System::String ^> ^args)
{
	try
	{
		String^ path = FolderConfigHelper::GetConfigPath();

		Console::WriteLine(String::Format("path is \"{0}\".",path));
		Console::WriteLine(String::Format("Path is acceesable...? {0}", FolderConfigHelper::IsFolderAccessable(path)));


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
	}
	catch(Exception^ ex)
	{
		Console::Error->WriteLine(ex->Message);
	}
	
	return 0;
}
