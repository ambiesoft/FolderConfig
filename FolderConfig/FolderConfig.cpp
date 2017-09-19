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

using namespace System::Diagnostics;

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

// up to 15 byte
enum ErrorReturnValue {
	ErrorReturn_NoError= 0,
	ErrorReturn_SettingsInitFailed ,
	ErrorReturn_ParseFailed,
	ErrorReturn_RunAsFailed,
};

HRESULT MakeRetval(bool success, WORD errorCode, bool okcancel)
{
	return HRESULT( 
		((unsigned long)(success?0:1))		<<31 		|
		((unsigned long)1)				<<29 		| // customer code (0x20000000)
		((unsigned long)errorCode)		<<1  		| // errorCode
		((unsigned long)(okcancel?1:0))
		);
}

int libmain(array<System::String ^> ^args)
{
	if (!Settings::init())
	{
		return MakeRetval(false, ErrorReturn_SettingsInitFailed, false);
	}

	if(!PowWrite::IsAdmin() && !PowWrite::do_c_write(Settings::UserIniFullpath))
	{
		// could not write to inifile, launch me in higher priviledge
		try
		{
			System::Diagnostics::ProcessStartInfo psi;
			psi.UseShellExecute=true;
			psi.WorkingDirectory = System::Environment::CurrentDirectory;
			psi.FileName = Application::ExecutablePath;
			psi.Arguments = PowWrite::getArgForCreate(args);
			psi.Verb = L"runas";
			Process^ pro = Process::Start(%psi);
			pro->WaitForExit();
			
			DTRACE_RETVAL(pro->ExitCode);
			return pro->ExitCode;
		}
		catch(System::Exception^ ex)
		{
			MessageBox::Show(
				ex->Message,
				Settings::Title,
				System::Windows::Forms::MessageBoxButtons::OK,
				System::Windows::Forms::MessageBoxIcon::Error);

			return MakeRetval(false,ErrorReturn_RunAsFailed,false);
		}
	}


	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	FormMain dlg;
	System::Windows::Forms::DialogResult dr = dlg.ShowDialog();

	return MakeRetval(true, ErrorReturn_NoError, dr != System::Windows::Forms::DialogResult::Cancel);
}

public ref struct Program
{
	static int main(array<System::String ^> ^args)
	{
		int ret = libmain(args);
#ifdef _DEBUG_RETVAL
		StringBuilder sb;
		DTRACE_RETVAL(ret);
		sb.AppendLine(String::Format(L"Succeeded={0}", SUCCEEDED(ret) ? "true":"false"));
		sb.AppendLine(String::Format(L"Customer={0}", (ret & 0x20000000) ? "true":"false"));
		sb.AppendLine("HRESULT_FACILITY: " + HRESULT_FACILITY(ret));

		WORD code = HRESULT_CODE(ret);
		sb.AppendLine("SCODE_CODE: " + code);
		sb.AppendLine("MyError: " + (code>>1));
		sb.AppendLine("OkCancel: " + ((ret&1)?"OK":"Cancel"));
		DTRACE_RETVAL(sb.ToString());
		MessageBox::Show(sb.ToString(), "_DEBUG_RETVAL" + (PowWrite::IsAdmin()?"(Admin)":"(Normal)"));
#endif
		return ret;
	}
};

} }