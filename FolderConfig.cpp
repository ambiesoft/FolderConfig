#include "stdafx.h"
#include "Form1.h"

#include "Settings.h"

#include "../MyUtility/CommandLineParser.h"
#include "../MyUtility/IsFileNamable.h"
#pragma comment(lib, "Shell32.lib")

using namespace std;
using namespace Ambiesoft;
using namespace Ambiesoft::FolderConfig;



ref struct PowWrite
{
	literal String^ CUSTOMER_TID_APP = L"Customer";
	literal String^ CUSTOMER_TID_KEY = L"TID";

	static bool IsAdmin()
	{
		try
		{
			System::Security::Principal::WindowsIdentity^ wi = System::Security::Principal::WindowsIdentity::GetCurrent();
			System::Security::Principal::WindowsPrincipal^ wp = gcnew System::Security::Principal::WindowsPrincipal(wi);
			return wp->IsInRole(System::Security::Principal::WindowsBuiltInRole::Administrator);
		}
		catch(System::Exception^)
		{
		}
		return false;
	}
	static bool do_c_write(String^ iniFileName)
	{
		int val = System::Environment::TickCount;

		Ambiesoft::HashIni^ hi = Ambiesoft::Profile::ReadAll(iniFileName);
		Ambiesoft::Profile::WriteInt(
			CUSTOMER_TID_APP,
			CUSTOMER_TID_KEY,
			val,
			hi);
		Ambiesoft::Profile::WriteAll(hi,iniFileName);



		int readv;
		Ambiesoft::Profile::GetInt(
			CUSTOMER_TID_APP,
			CUSTOMER_TID_KEY,
			-1,
			readv,
			iniFileName);

		return val==readv;
	}

	static String^ getArgForCreate(array<System::String^>^ args)
	{
		System::Text::StringBuilder sb;
		for each(String^ s in args)
		{
			if(s->IndexOf(L' ') > 0)
				sb.Append(L"\"" + s + L"\"");
			else
				sb.Append(s);

			sb.Append(L" ");
		}
		return sb.ToString()->TrimEnd();
	}
};

String^ getHelpMessage()
{
	return I18N(
		"Usage:\r\n"
		"FolderConfig.exe [/title title] /inifile inifile [/defaultpath defaultpath] /creator creator /appname appname\r\n"
		"\r\n"
		"title: Title shown in Titlebar.\r\n"
		"inifile: filename user chosen data will be saved.\r\n"
		"defaultpath: Default path of inifile.\r\n"
		"creator: Creator name used as subfolder of roaming or local folder.\r\n"
		"appname: App name user as subfolder of creator.");

}
void ErrorExit(String^ s)
{

	MessageBox::Show(s + "\r\n\r\n" + getHelpMessage(),
		Application::ProductName,
		MessageBoxButtons::OK,
		MessageBoxIcon::Error);

	System::Environment::Exit(1);
}

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	CCommandLineParser parser;
	COption opTitle(L"/title", 1);
	parser.AddOption(&opTitle);

	COption opIniFile(L"/inifile", 1);
	parser.AddOption(&opIniFile);

	COption opDefaultPath(L"/defaultpath", 1);
	parser.AddOption(&opDefaultPath);

	COption opCreator(L"/creator", 1);
	parser.AddOption(&opCreator);

	COption opAppName(L"/appname", 1);
	parser.AddOption(&opAppName);

	parser.Parse();

	if(opTitle.hadOption())
	{
		Settings::title = gcnew String(opTitle.getValueStrings().c_str());
	}
	if(String::IsNullOrEmpty(Settings::title))
		Settings::title = Application::ProductName;

	// save file
	if(opIniFile.hadOption())
	{
		if(opIniFile.getValueCount() > 1)
		{
			ErrorExit(I18N(String::Format(L"{0} ini files specified. Only one acceptable.", 
				opIniFile.getValueCount())));
		}
		if(!IsFullPathNamble(opIniFile.getValueStrings().c_str()))
		{
			ErrorExit(I18N(String::Format(L"{0} must not include {1}.",
				opIniFile.getValueCount(), GetFullPathInamableCharsCLR())));
		}
		Settings::iniFileName=gcnew String(opIniFile.getValueStrings().c_str());
	}
	if(String::IsNullOrEmpty(Settings::iniFileName))
	{
		ErrorExit(I18N(L"No ini file specified"));
	}


	// default path
	if(opDefaultPath.hadOption())
	{
		if(opDefaultPath.getValueCount() > 1)
		{
			ErrorExit(I18N(String::Format(L"{0} default path specified. Only one acceptable.", 
				opDefaultPath.getValueCount())));
		}
		if(!IsFullPathNamble(opDefaultPath.getValueStrings().c_str()))
		{
			ErrorExit(I18N(String::Format(L"{0} must not include {1}.",
				"defaultpath", GetFullPathInamableCharsCLR())));
		}
		Settings::DefaultPath = gcnew String(opDefaultPath.getValueStrings().c_str());
	}
	// default path is nullable


	// creator
	if(opCreator.hadOption())
	{
		if(opCreator.getValueCount() > 1)
		{
			ErrorExit(I18N(String::Format(L"{0} default path specified. Only one acceptable.", 
				opCreator.getValueCount())));
		}
		if(!IsFileNamble(opCreator.getValueStrings().c_str()))
		{
			ErrorExit(I18N(String::Format(L"{0} must not include {1}.",
				"creator", GetFileInamableCharsCLR())));
		}
		Settings::Creator = gcnew String(opCreator.getValueStrings().c_str());
	}
	if(String::IsNullOrEmpty(Settings::Creator))
	{
		ErrorExit(I18N(L"Creator not specified."));
	}
	
	// appname
	if(opAppName.hadOption())
	{
		if(opAppName.getValueCount() > 1)
		{
			ErrorExit(I18N(String::Format(L"{0} default path specified. Only one acceptable.", 
				opAppName.getValueCount())));
		}
		if(!IsFileNamble(opAppName.getValueStrings().c_str()))
		{
			ErrorExit(I18N(String::Format(L"{0} must not include {1}.",
				L"appname", GetFileInamableCharsCLR())));
		}
		Settings::AppName = gcnew String(opAppName.getValueStrings().c_str());
	}
	if(String::IsNullOrEmpty(Settings::AppName))
	{
		ErrorExit(I18N(L"AppName not specified."));
	}


	if(!PowWrite::IsAdmin() && !PowWrite::do_c_write(Settings::iniFileName))
	{
		System::Diagnostics::ProcessStartInfo psi;
		psi.UseShellExecute=true;
		psi.WorkingDirectory = System::Environment::CurrentDirectory;
		psi.FileName = Application::ExecutablePath;
		psi.Arguments = PowWrite::getArgForCreate(args);
		psi.Verb = L"runas";
		try
		{
			System::Diagnostics::Process::Start(%psi);
		}
		catch(System::Exception^ ex)
		{
			MessageBox::Show(
				ex->Message,
				Settings::Title,
				System::Windows::Forms::MessageBoxButtons::OK,
				System::Windows::Forms::MessageBoxIcon::Error);

		}
		return 0;
	}


	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	Form1 dlg;

	dlg.ShowDialog();

	return 0;
}
