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
#include "Settings.h"

#include "../../lsMisc/CommandLineParser.h"
#include "../../lsMisc/stdwin32/stdwin32.h"

using std::wstring;

namespace Ambiesoft {
	namespace FolderConfig {

		using namespace stdwin32;

		using namespace System::IO;
		using namespace System::Text;
		using namespace System::Windows::Forms;


		String^ getHelpMessage()
		{
			StringBuilder sb;
			sb.AppendLine(I18N(L"Usage:"));
			sb.AppendLine(L"FolderConfig.exe [/section Section] [/title Title] [/inifile Inifile] /defaultpathtype Defaultpathtype [/defaultpath0 Defaultpath0] [/defaultpath3 Defaultpath3] [/creator Creator] [/appname Appname] [/culture Culture]");
			sb.AppendLine();

			sb.AppendLine(I18N(L"Section: Section name of config. (Default is [Main])"));
			
			sb.AppendLine(I18N(L"Title: Title shown in Titlebar."));
			sb.AppendLine(I18N(L"Inifile: filename user chosen data will be saved. (Default is folder.ini)"));

			sb.AppendLine(I18N(L"Defaultpathtype: 0,1,2 or 3."));
			sb.AppendLine(I18N(L"Defaultpath0: Default relative path from exe-resident folder for pathtype of 0. (Default is Exe-resident folder)"));
			sb.AppendLine(I18N(L"Defaultpath3: Default full path for pathytpe of 3."));
			sb.AppendLine(I18N(L"Creator: Creator name used as subfolder of roaming or local folder."));
			sb.AppendLine(I18N(L"Appname: App name used as subfolder of Creator."));
			
			sb.AppendLine(I18N(L"Culture: Culture string, (ex) \"ja-JP\"."));

			return sb.ToString();
		}

		void ShowError(String^ s)
		{
			MessageBox::Show(s + "\r\n\r\n" + getHelpMessage(),
				Settings::ProductName,
				MessageBoxButtons::OK,
				MessageBoxIcon::Error);
		}
		void Alert(String^ msg)
		{
			MessageBox::Show(msg,
				Settings::ProductName,
				MessageBoxButtons::OK,
				MessageBoxIcon::Warning);
		}
		void Alert(Exception^ ex)
		{
			Alert(ex->Message);
		}
		bool Settings::init()
		{
			CCommandLineParser parser(CaseFlags_Insensitive);
			COption opTitle(L"/title", 1);
			parser.AddOption(&opTitle);

			//COption opIniFile(L"/inifile", 1);
			//parser.AddOption(&opIniFile);

			COption opDefaultPathtype(L"/defaultpathtype", 1);
			parser.AddOption(&opDefaultPathtype);

			COption opDefaultPath0(L"/defaultpath0", 1);
			parser.AddOption(&opDefaultPath0);

			COption opDefaultPath3(L"/defaultpath3", 1);
			parser.AddOption(&opDefaultPath3);

			COption opCreator(L"/creator", 1);
			parser.AddOption(&opCreator);

			COption opAppName(L"/appname", 1);
			parser.AddOption(&opAppName);

			COption opSection(L"/section", 1);
			parser.AddOption(&opSection);

			COption opCulture(L"/culture", 1);
			parser.AddOption(&opCulture);

			COption opHelp(L"/h", L"/?");
			parser.AddOption(&opHelp);


			parser.Parse();

			String^ culture;

			String^ inipath = Path::Combine(Path::GetDirectoryName(Application::ExecutablePath),
				ProductName + L".ini");
			HashIni^ ini = Profile::ReadAll(inipath);
			
			// section
			if (opSection.hadOption())
			{
				if (opSection.getValueCount() > 1)
				{
					ShowError(
						I18N(String::Format(L"{0} sections specified. Only one acceptable.",
						opSection.getValueCount())));
					return false;
				}
				Settings::section_ = gcnew String(opSection.getValueStrings().c_str());
			}
			if (String::IsNullOrEmpty(Settings::Section))
			{
				Settings::section_ = Settings::section_default_;
			}

			Profile::GetString(Section, "title", nullptr, title_, ini);
			Profile::GetString(Section, "appname", nullptr, appName_, ini);
			Profile::GetString(Section, "creator", nullptr, creator_, ini);
			Profile::GetInt(Section, "defaultpathtype", -1, defaultpathtype_, ini);
			Profile::GetString(Section, "defaultpath0", nullptr, defaultpath0_, ini);
			Profile::GetString(Section, "defaultpath3", nullptr, defaultpath3_, ini);
			Profile::GetString(Section, "culture", nullptr, culture, ini);
#ifdef _DEBUG
			if(!String::IsNullOrEmpty(title_))
				System::Diagnostics::Debug::Print(L"title is \"{0}\" (from \"{1}\")",title_,inipath);
			if(!String::IsNullOrEmpty(appName_))
				System::Diagnostics::Debug::Print(L"Appname is \"{0}\" (from \"{1}\")",appName_,inipath);
			if(!String::IsNullOrEmpty(creator_))
				System::Diagnostics::Debug::Print(L"Creator is \"{0}\" (from \"{1}\")",creator_,inipath);
			if(defaultpathtype_ >=0)
				System::Diagnostics::Debug::Print(L"defaultpathtype is \"{0}\" (from \"{1}\")",defaultpathtype_,inipath);
			if(!String::IsNullOrEmpty(defaultpath0_))
				System::Diagnostics::Debug::Print(L"defaultpath0_ is \"{0}\" (from \"{1}\")",defaultpath0_,inipath);
			if(!String::IsNullOrEmpty(defaultpath3_))
				System::Diagnostics::Debug::Print(L"defaultpath3_ is \"{0}\" (from \"{1}\")",defaultpath3_,inipath);
			if(!String::IsNullOrEmpty(culture))
				System::Diagnostics::Debug::Print(L"culture is \"{0}\" (from \"{1}\")",culture,inipath);
#endif

			// First, set culture for I18N
			if(opCulture.hadOption())
			{
				culture = gcnew String(opCulture.getFirstValue().c_str());
			}
			if(!String::IsNullOrEmpty(culture))
			{
				try
				{
					System::Globalization::CultureInfo^ ci = gcnew System::Globalization::CultureInfo(culture);
					System::Threading::Thread::CurrentThread->CurrentCulture = ci;
					System::Threading::Thread::CurrentThread->CurrentUICulture = ci;
				}
				catch(Exception^ ex)
				{
					Alert(ex);
				}
			}


			if(opHelp.hadOption())
			{
				MessageBox::Show(getHelpMessage(),
					ProductName,
					MessageBoxButtons::OK,
					MessageBoxIcon::Information
					);
				return 0;
			}

			if (opTitle.hadOption())
			{
				Settings::title_ = gcnew String(opTitle.getValueStrings().c_str());
			}

			// save file
			//if (opIniFile.hadOption())
			//{
			//	if (opIniFile.getValueCount() > 1)
			//	{
			//		ErrorExit(I18N(String::Format(L"{0} ini files specified. Only one acceptable.",
			//			opIniFile.getValueCount())));
			//	}


			//	if (!IsFullPathNamble(opIniFile.getValueStrings().c_str()))
			//	{
			//		ErrorExit(I18N(String::Format(L"{0} must not include {1}.",
			//			toCLR(opIniFile.getValueStrings()), GetFullPathInamableCharsCLR())));
			//	}

			//	String^ dir = System::IO::Directory::GetParent(System::Windows::Forms::Application::ExecutablePath)->FullName;
			//	String^ file = gcnew String(opIniFile.getValueStrings().c_str());
			//	String^ fullpath = System::IO::Path::Combine(dir, file);
			//	Settings::iniFileName_ = fullpath;
			//}
			//else
			//{
			//	Settings::iniFileName_ = Settings::DefaultInifileName;
			//}
			//if (String::IsNullOrEmpty(Settings::iniFileName_))
			//{
			//	ErrorExit(I18N(L"No ini file specified"));
			//}

			if(opDefaultPathtype.hadOption())
			{
				wstring ws = opDefaultPathtype.getFirstValue();
				if(ws != L"0" &&
					ws != L"1" &&
					ws != L"2" &&
					ws != L"3" )
				{
					ShowError(I18N(L"Defaultpathtype must be 0,1,2 or 3."));
					return false;
				}
				defaultpathtype_ = _wtoi(ws.c_str());
			}
			if (defaultpathtype_ < 0)
			{
				ShowError(I18N(L"Defaultpathtype must be specified."));
				return false;
			}


			// default path
			if (opDefaultPath0.hadOption())
			{
				if (opDefaultPath0.getValueCount() > 1)
				{
					ShowError(
						I18N(String::Format(L"{0} default paths specified. Only one acceptable.",
						opDefaultPath0.getValueCount())));
					return false;
				}
				if (!IsRelativePathNamble(opDefaultPath0.getValueStrings().c_str()))
				{
					ShowError(
						I18N(String::Format(L"{0} must not include {1}.",
						"defaultpath", toCLR(GetRelativePathInamableChars()))));
					return false;
				}
				defaultpath0_ = gcnew String(opDefaultPath0.getValueStrings().c_str());
			}
			if(!String::IsNullOrEmpty(defaultpath0_))
			{
				// must be relative path
				if(!PathIsRelative(toLPCW(defaultpath0_)))
				{
					ShowError(String::Format(I18N(L"Defaultpath0 \"{0}\" must be a relative path."), defaultpath0_));
					return false;
				}
			}

			if (opDefaultPath3.hadOption())
			{
				defaultpath3_ = gcnew String(opDefaultPath3.getValueStrings().c_str());
			}
			if(!String::IsNullOrEmpty(defaultpath3_))
			{
				// must be relative path
				if(PathIsRelative(toLPCW(defaultpath3_)))
				{
					ShowError(String::Format(I18N(L"Defaultpath3 \"{0}\" must be a full path."), defaultpath3_));
					return false;
				}
			}


			// creator
			if (opCreator.hadOption())
			{
				if (opCreator.getValueCount() > 1)
				{
					ShowError(
						I18N(String::Format(L"{0} creators path specified. Only one acceptable.",
						opCreator.getValueCount())));
					return false;
				}
				if (!IsFileNamble(opCreator.getValueStrings().c_str()))
				{
					ShowError(
						I18N(String::Format(L"{0} must not include {1}.",
						"creator", GetFileInamableCharsCLR())));
					return false;
				}
				Settings::creator_ = gcnew String(opCreator.getValueStrings().c_str());
			}
			//else
			//{
			//	Settings::creator_ = Settings::DefaultCreator;
			//}

			if (String::IsNullOrEmpty(Settings::Creator))
			{
				ShowError(I18N(L"Creator must be specified."));
				return false;
			}


			// appname
			if (opAppName.hadOption())
			{
				if (opAppName.getValueCount() > 1)
				{
					ShowError(
						I18N(String::Format(L"{0} appnames specified. Only one acceptable.",
						opAppName.getValueCount())));
					return false;
				}
				if (!IsFileNamble(opAppName.getValueStrings().c_str()))
				{
					ShowError(
						I18N(String::Format(L"{0} must not include {1}.",
						L"appname", GetFileInamableCharsCLR())));
					return false;
				}
				Settings::appName_ = gcnew String(opAppName.getValueStrings().c_str());
			}
			//else
			//{
			//	Settings::appName_ = Settings::DefaultAppName;
			//}
			if (String::IsNullOrEmpty(Settings::AppName))
			{
				ShowError(I18N(L"Appname must be specified."));
				return false;
			}






			// check unknown option
			if (parser.hadUnknownOption())
			{
				StringBuilder sb;
				sb.AppendLine(I18N(L"Unknown option:"));
				sb.AppendLine(gcnew String(parser.getUnknowOptionStrings().c_str()));
				ShowError(sb.ToString());
				return false;
			}

			return true;

		}

		//String^ Settings::InifileName::get()
		//{
		//	DASSERT(!String::IsNullOrEmpty(iniFileName_));
		//	DASSERT(stdIsFullPath(toLPCW(iniFileName_)));

		//	return iniFileName_;
		//}

		String^ Settings::DefaultPath0::get()
		{
			if (String::IsNullOrEmpty(defaultpath0_))
				return toCLR(stdGetParentDirectory(stdGetModuleFileName()));

			if (!stdIsFullPath(toLPCW(defaultpath0_)))
				return toCLR(stdCombinePath(stdGetParentDirectory(stdGetModuleFileName()), toWstring(defaultpath0_)));

			return defaultpath0_;
		}

	}
}