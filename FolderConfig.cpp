#include "stdafx.h"
#include "FormMain.h"

#include "Settings.h"

#include "../lsMisc/CommandLineParser.h"
#include "../lsMisc/IsFileNamable.h"
#pragma comment(lib, "Shell32.lib")


using namespace Ambiesoft;
using namespace Ambiesoft::FolderConfig;
using namespace System::Text;

using std::wstring;



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





enum ErrorReturnValue {
	ErrorReturn_NoError = 0,
	ErrorReturn_SettingsInitFailed ,
	ErrorReturn_ParseFailed,
	ErrorReturn_RunAsFailed,
};

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	if (!Settings::init())
		return ErrorReturn_SettingsInitFailed;

	if(!PowWrite::IsAdmin() && !PowWrite::do_c_write(Settings::UserIniFullpath))
	{
		// could not write to inifile, launch me in higher priviledge
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

			return ErrorReturn_RunAsFailed;
		}
		return ErrorReturn_NoError;
	}


	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	FormMain dlg;

	dlg.ShowDialog();

	return ErrorReturn_NoError;
}
