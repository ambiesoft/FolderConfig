//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#include "stdafx.h"
#include "FormMain.h"

#include "Settings.h"

#include "../../lsMisc/CommandLineParser.h"
#include "../../lsMisc/IsFileNamable.h"
#pragma comment(lib, "Shell32.lib")


using namespace Ambiesoft;
using namespace Ambiesoft::FolderConfig;
using namespace System::Text;

using std::wstring;


namespace Ambiesoft { namespace FolderConfig {

ref struct PowWrite
{
	literal String^ CUSTOMER_TID_APP = L"TestWrite";
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


int libmain(array<System::String ^> ^args)
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

public ref struct Program
{
	static int main(array<System::String ^> ^args)
	{
		return libmain(args);
	}
};

} }