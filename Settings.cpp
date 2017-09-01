#include "stdafx.h"
#include "Settings.h"

#include "../MyUtility/CommandLineParser.h"
#include "../MyUtility/stdwin32/stdwin32.h"



namespace Ambiesoft {
	namespace FolderConfig {

		using namespace stdwin32;

		using namespace System::IO;
		using namespace System::Text;
		using namespace System::Windows::Forms;

		String^ getHelpMessage()
		{
			return I18N(
				"Usage:\r\n"
				"FolderConfig.exe [/title title] /inifile inifile [/defaultpath defaultpath] /creator creator /appname appname [/section section]\r\n"
				"\r\n"
				"title: Title shown in Titlebar.\r\n"
				// "inifile: filename user chosen data will be saved.\r\n"
				"defaultpath: Default relative folder path from exe-resident folder,\r\n"
				"\tThis folder is default location when no setting is available.\r\n"
				"creator: Creator name used as subfolder of roaming or local folder.\r\n"
				"appname: App name user as subfolder of creator."
				"section: Section name of config"
				);

		}

		void ErrorExit(String^ s)
		{
			MessageBox::Show(s + "\r\n\r\n" + getHelpMessage(),
				Application::ProductName,
				MessageBoxButtons::OK,
				MessageBoxIcon::Error);

			System::Environment::Exit(1);
		}


		bool Settings::init()
		{
			CCommandLineParser parser;
			COption opTitle(L"/title", 1);
			parser.AddOption(&opTitle);

			//COption opIniFile(L"/inifile", 1);
			//parser.AddOption(&opIniFile);

			COption opDefaultPath(L"/defaultpath", 1);
			parser.AddOption(&opDefaultPath);

			COption opCreator(L"/creator", 1);
			parser.AddOption(&opCreator);

			COption opAppName(L"/appname", 1);
			parser.AddOption(&opAppName);

			COption opSection(L"/section", 1);
			parser.AddOption(&opSection);

			COption opHelp(L"/h", L"/?");
			parser.AddOption(&opHelp);


			parser.Parse();

			if(opHelp.hadOption())
			{
				MessageBox::Show(getHelpMessage(),
					Application::ProductName,
					MessageBoxButtons::OK,
					MessageBoxIcon::Information
					);
				return 0;
			}

			if (opTitle.hadOption())
			{
				Settings::title_ = gcnew String(opTitle.getValueStrings().c_str());
			}
			if (String::IsNullOrEmpty(Settings::title_))
				Settings::title_ = Application::ProductName;

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


			// default path
			if (opDefaultPath.hadOption())
			{
				if (opDefaultPath.getValueCount() > 1)
				{
					ErrorExit(I18N(String::Format(L"{0} default paths specified. Only one acceptable.",
						opDefaultPath.getValueCount())));
				}
				if (!IsFileNamble(opDefaultPath.getValueStrings().c_str()))
				{
					ErrorExit(I18N(String::Format(L"{0} must not include {1}.",
						"defaultpath", GetFileInamableCharsCLR())));
				}
				Settings::defaultUserPath_ = gcnew String(opDefaultPath.getValueStrings().c_str());
			}
			// default path is nullable


			// creator
			if (opCreator.hadOption())
			{
				if (opCreator.getValueCount() > 1)
				{
					ErrorExit(I18N(String::Format(L"{0} creators path specified. Only one acceptable.",
						opCreator.getValueCount())));
				}
				if (!IsFileNamble(opCreator.getValueStrings().c_str()))
				{
					ErrorExit(I18N(String::Format(L"{0} must not include {1}.",
						"creator", GetFileInamableCharsCLR())));
				}
				Settings::creator_ = gcnew String(opCreator.getValueStrings().c_str());
			}
			else
			{
				Settings::creator_ = Settings::DefaultCreator;
			}

			if (String::IsNullOrEmpty(Settings::Creator))
			{
				ErrorExit(I18N(L"Creator not specified."));
			}


			// appname
			if (opAppName.hadOption())
			{
				if (opAppName.getValueCount() > 1)
				{
					ErrorExit(I18N(String::Format(L"{0} appnames specified. Only one acceptable.",
						opAppName.getValueCount())));
				}
				if (!IsFileNamble(opAppName.getValueStrings().c_str()))
				{
					ErrorExit(I18N(String::Format(L"{0} must not include {1}.",
						L"appname", GetFileInamableCharsCLR())));
				}
				Settings::appName_ = gcnew String(opAppName.getValueStrings().c_str());
			}
			else
			{
				Settings::appName_ = Settings::DefaultAppName;
			}
			if (String::IsNullOrEmpty(Settings::AppName))
			{
				ErrorExit(I18N(L"AppName not specified."));
			}


			// section
			if (opSection.hadOption())
			{
				if (opSection.getValueCount() > 1)
				{
					ErrorExit(I18N(String::Format(L"{0} appnames specified. Only one acceptable.",
						opSection.getValueCount())));
				}
				Settings::section_ = gcnew String(opSection.getValueStrings().c_str());
			}
			if (String::IsNullOrEmpty(Settings::Section))
			{
				Settings::section_ = L"FolderConfig";
			}


			// check unknown option
			if (parser.hadUnknownOption())
			{
				StringBuilder sb;
				sb.AppendLine(I18N(L"Unknown option:"));
				sb.AppendLine(gcnew String(parser.getUnknowOptionStrings().c_str()));
				ErrorExit(sb.ToString());
			}

			return true;

		}

		//String^ Settings::InifileName::get()
		//{
		//	DASSERT(!String::IsNullOrEmpty(iniFileName_));
		//	DASSERT(stdIsFullPath(toLPCW(iniFileName_)));

		//	return iniFileName_;
		//}

		String^ Settings::DefaultUserPath::get()
		{
			if (String::IsNullOrEmpty(defaultUserPath_))
				return toCLR(stdGetParentDirectory(stdGetModuleFileName()));

			if (!stdIsFullPath(toLPCW(defaultUserPath_)))
				return toCLR(stdCombinePath(stdGetParentDirectory(stdGetModuleFileName()), toWstring(defaultUserPath_)));

			return defaultUserPath_;
		}
		
	}
}