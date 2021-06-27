// TestApp.cpp : main project file.

#include "stdafx.h"

#include "../../lsMisc/stdwin32/stdwin32.h"
#include "../../lsMisc/UTF16toUTF8.h"
#include "../../lsMisc/stdosd/stdosd.h"

using namespace System;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;

using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;
using namespace stdwin32;

using std::wstring;

void CheckTrue(bool b)
{
	if (!b)
		MessageBox::Show("error");
}
String^ GetFolderConfigExe()
{
	String^ folconexe = Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location);
	folconexe = Path::Combine(folconexe, "FolderConfig.exe");
	return folconexe;
}
void LaunchAndWait(String^ commandline)
{
	Process^ pro = Process::Start(GetFolderConfigExe(), commandline);
	pro->WaitForExit();
}
void preCheck()
{
	String^ proj = File::ReadAllText("FolderConfig.ini");
	String^ distFile = Path::Combine(
		Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location), 
		"FolderConfig.ini");
	String^ dist = File::ReadAllText(distFile);
	if (proj != dist)
	{
		MessageBox::Show("Not Ready");
		Environment::Exit(1);
	}
}
String^ GetWin32Values(const wchar_t* section, 
	const wchar_t* creater = NULL,
	const wchar_t* appname = NULL)
{
	const wstring iniFolderConfig = stdCombinePath(stdGetParentDirectory(stdGetModuleFileName<wchar_t>()), L"FolderConfig.ini");
	WCHAR defaultcreator[MAX_PATH];
	GetPrivateProfileString(section, L"creator", L"", defaultcreator, _countof(defaultcreator), iniFolderConfig.c_str());

	WCHAR defaultappname[MAX_PATH];
	GetPrivateProfileString(section, L"appname", L"", defaultappname, _countof(defaultappname), iniFolderConfig.c_str());

	WCHAR defaultpath0[MAX_PATH];
	GetPrivateProfileString(section, L"defaultpath0", L"", defaultpath0, _countof(defaultpath0), iniFolderConfig.c_str());

	WCHAR defaultpath3[MAX_PATH];
	GetPrivateProfileString(section, L"defaultpath3", L"", defaultpath3, _countof(defaultpath3), iniFolderConfig.c_str());

	if (!creater)
		creater = defaultcreator;
	if (!appname)
		appname = defaultappname;

	const wstring iniFolder = stdCombinePath(stdGetParentDirectory(stdGetModuleFileName<wchar_t>()), L"folder.ini");
	const int pathtype = GetPrivateProfileInt(section, L"PathType", 0, iniFolder.c_str());
	WCHAR buff[MAX_PATH] = { 0 };
	switch (pathtype)
	{
	case 0:
		return gcnew String(stdCombinePath(
			stdGetParentDirectory(stdGetModuleFileName()),
			defaultpath0).c_str());
	case 1:
		SHGetSpecialFolderPath(NULL, buff, CSIDL_LOCAL_APPDATA, FALSE);
		return gcnew String(stdCombinePath(stdCombinePath(buff, creater), appname).c_str());
	case 2:
		SHGetSpecialFolderPath(NULL, buff, CSIDL_APPDATA, FALSE);
		return gcnew String(stdCombinePath(stdCombinePath(buff, creater), appname).c_str());
	case 3:
		return gcnew String(defaultpath3);
	}
	MessageBox::Show("error");
	return String::Empty;
}
int main(array<System::String ^> ^args)
{
	preCheck();
	try
	{
		{
			LaunchAndWait(String::Format(
				"/nookmessage /debug \"{0}\"",
				"select:0,click:OK"));
			String^ normalConfigPath = FolderConfigHelper::GetConfigPath();
			CheckTrue(!String::IsNullOrEmpty(normalConfigPath));
			CheckTrue(FolderConfigHelper::IsFolderAccessible(normalConfigPath));
			String^ checkNormalConfigPath = Path::Combine(
				Path::GetDirectoryName(Application::ExecutablePath),
				"data");
			CheckTrue(normalConfigPath == checkNormalConfigPath);

			String^ win32Value = GetWin32Values(L"Main");
			CheckTrue(win32Value == checkNormalConfigPath);
		}
		{
			LaunchAndWait(String::Format(
				"/nookmessage /debug \"{0}\"",
				"select:1,click:OK"));
			String^ normalConfigPath = FolderConfigHelper::GetConfigPath();
			CheckTrue(!String::IsNullOrEmpty(normalConfigPath));
			CheckTrue(FolderConfigHelper::IsFolderAccessible(normalConfigPath));
			String^ checkNormalConfigPath = Path::Combine(
				Environment::GetFolderPath(Environment::SpecialFolder::LocalApplicationData),
				"Ambiesoft",
				"TestApp");
			CheckTrue(normalConfigPath == checkNormalConfigPath);

			String^ win32Value = GetWin32Values(L"Main");
			CheckTrue(win32Value == checkNormalConfigPath);
		}
		{
			LaunchAndWait(String::Format(
				"/nookmessage /debug \"{0}\"",
				"select:2,click:OK"));
			String^ normalConfigPath = FolderConfigHelper::GetConfigPath();
			CheckTrue(!String::IsNullOrEmpty(normalConfigPath));
			CheckTrue(FolderConfigHelper::IsFolderAccessible(normalConfigPath));
			String^ checkNormalConfigPath = Path::Combine(
				Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData),
				"Ambiesoft",
				"TestApp");
			CheckTrue(normalConfigPath == checkNormalConfigPath);

			String^ win32Value = GetWin32Values(L"Main");
			CheckTrue(win32Value == checkNormalConfigPath);
		}
		{
			LaunchAndWait(String::Format(
				"/nookmessage /debug \"{0}\"",
				"select:3,click:OK"));
			String^ normalConfigPath = FolderConfigHelper::GetConfigPath();
			CheckTrue(!String::IsNullOrEmpty(normalConfigPath));
			// CheckTrue(FolderConfigHelper::IsFolderAccessible(normalConfigPath));
			String^ checkNormalConfigPath = "L:\\aaa\\eee\\";
			CheckTrue(normalConfigPath == checkNormalConfigPath);

			String^ win32Value = GetWin32Values(L"Main");
			CheckTrue(win32Value == checkNormalConfigPath);
		}




		// Ninja settings
		{
			String^ cmdline = String::Format("/section {0} /defaultpathtype {1} /creator Ambiesoft /appname MyApp /nookmessage /debug \"{2}\"",
				"Ninja",
				2,
				"select:0,click:OK");
			LaunchAndWait(cmdline);
			String^ ninjaPath1 = FolderConfigHelper::GetConfigPath("Ninja");
			String^ ninjaPath2 = FolderConfigHelper::GetConfigPath("Ninja", "NazoCompany", "MyApp");
			String^ checkNinjaPath = Path::Combine(
				Path::GetDirectoryName(Application::ExecutablePath),
				"ninjadata");
			CheckTrue(ninjaPath1 == checkNinjaPath);
			CheckTrue(ninjaPath2 == checkNinjaPath);

			String^ win32Value1 = GetWin32Values(L"Ninja");
			String^ win32Value2 = GetWin32Values(L"Ninja", L"NazoCompany", L"MyApp");
			CheckTrue(win32Value1 == checkNinjaPath);
			CheckTrue(win32Value2 == checkNinjaPath);
		}
		{
			String^ cmdline = String::Format("/section {0} /defaultpathtype {1} /creator Ambiesoft /appname MyApp /nookmessage /debug \"{2}\"",
				"Ninja",
				2,
				"select:1,click:OK");
			LaunchAndWait(cmdline);
			{
				String^ ninjaPath = FolderConfigHelper::GetConfigPath("Ninja");
				String^ checkNinjaPath = Path::Combine(
					Environment::GetFolderPath(Environment::SpecialFolder::LocalApplicationData),
					"Ambiesoft",
					"TestApp_Ninja");
				CheckTrue(ninjaPath == checkNinjaPath);

				String^ win32Value = GetWin32Values(L"Ninja");
				CheckTrue(win32Value == checkNinjaPath);
			}
			{
				String^ ninjaPath = FolderConfigHelper::GetConfigPath("Ninja", "NazoCompany", "MyApp");
				String^ checkNinjaPath = Path::Combine(
					Environment::GetFolderPath(Environment::SpecialFolder::LocalApplicationData),
					"NazoCompany",
					"MyApp");
				CheckTrue(ninjaPath == checkNinjaPath);
				
				String^ win32Value = GetWin32Values(L"Ninja", L"NazoCompany", L"MyApp");
				CheckTrue(win32Value == checkNinjaPath);
			}
		}
		{
			String^ cmdline = String::Format("/section {0} /defaultpathtype {1} /creator Ambiesoft /appname MyApp /nookmessage /debug \"{2}\"",
				"Ninja",
				2,
				"select:2,click:OK");
			LaunchAndWait(cmdline);
			{
				String^ ninjaPath = FolderConfigHelper::GetConfigPath("Ninja");
				String^ checkNinjaPath = Path::Combine(
					Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData),
					"Ambiesoft",
					"TestApp_Ninja");
				CheckTrue(ninjaPath == checkNinjaPath);

				String^ win32Value = GetWin32Values(L"Ninja");
				CheckTrue(win32Value == checkNinjaPath);
			}
			{
				String^ ninjaPath = FolderConfigHelper::GetConfigPath("Ninja", "NazoCompany", "MyApp");
				String^ checkNinjaPath = Path::Combine(
					Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData),
					"NazoCompany",
					"MyApp");
				CheckTrue(ninjaPath == checkNinjaPath);

				String^ win32Value = GetWin32Values(L"Ninja", L"NazoCompany", L"MyApp");
				CheckTrue(win32Value == checkNinjaPath);
			}
		}
		{
			String^ cmdline = String::Format("/section {0} /defaultpathtype {1} /creator Ambiesoft /appname MyApp /nookmessage /debug \"{2}\"",
				"Ninja",
				2,
				"select:3,click:OK");
			LaunchAndWait(cmdline);

			String^ ninjaPath1 = FolderConfigHelper::GetConfigPath("Ninja");
			String^ ninjaPath2 = FolderConfigHelper::GetConfigPath("Ninja", "NazoCompany", "MyApp");
			String^ checkNinjaPath = "L:\\aaa\\eee\\Ninja";
			CheckTrue(ninjaPath1 == checkNinjaPath);
			CheckTrue(ninjaPath2 == checkNinjaPath);

			String^ win32Value1 = GetWin32Values(L"Ninja");
			String^ win32Value2 = GetWin32Values(L"Ninja", L"NazoCompany", L"MyApp");
			CheckTrue(win32Value1 == checkNinjaPath);
			CheckTrue(win32Value2 == checkNinjaPath);
		}
	}
	catch(Exception^ ex)
	{
		Console::Error->WriteLine(ex->Message);
	}
	
	return 0;
}
