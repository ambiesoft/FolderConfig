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

#if _MSC_VER <= 1500
#define literal static initonly
#endif

namespace Ambiesoft { namespace FolderConfig {

	using namespace System;

	ref class Settings abstract sealed
	{
	public:
		literal String^ SEC_OPTION				= L"Option";
	private:
		static String^ title_;
		// static String^ iniFileName_;
		static String^ creator_;
		static String^ appName_;
		static String^ section_;
		
		literal String^ section_default_		= L"Main";
		static int defaultSelection_;
		
		static String^ defaultUserPath_;


		literal String^ userInifileName_ = L"folder.ini";

		static bool initialized_;

	public:
		static bool init();

		static property String^ Title
		{
			String^ get()
			{
				//if (String::IsNullOrEmpty(title_))
				//{
				//	return System::Windows::Forms::Application::ProductName;
				//}
				return title_;
			}
		}
		static property String^ Section
		{
			String^ get() { return section_; }
		}
		static property String^ ThisDir
		{
			String^ get()
			{
				return System::IO::Path::GetDirectoryName(System::Windows::Forms::Application::ExecutablePath);
			}
		}
		static property String^ UserIniFullpath
		{
			String^ get() 
			{
				return System::IO::Path::Combine(ThisDir, userInifileName_); 
			}
		}
		static property String^ Creator
		{
			String^ get() { return creator_; }
		}
		static property String^ AppName
		{
			String^ get() { return appName_; }
		}

		static property int DefaultSelection
		{
			int get() { return defaultSelection_; }
		}
		static property String^ DefaultUserPath
		{
			String^ get();
		}
		//static public bool initDefault()
		//{
		//	String^ dir = System::IO::Directory::GetParent(System::Windows::Forms::Application::ExecutablePath)->FullName;
		//	String^ definifile = System::IO::Path::Combine(dir, "folderconfig.ini");

		//	bool failed = false;
		//	Ambiesoft::HashIni^ defini = Ambiesoft::Profile::ReadAll(definifile);
		//	Ambiesoft::Profile::GetString("default", "title", "", Settings::title, defini);
		//	Ambiesoft::Profile::GetString("default", "inifile", "", Settings::iniFileName, defini);
		//	Ambiesoft::Profile::GetString("default", "creator", "", Settings::Creator, defini);
		//	Ambiesoft::Profile::GetString("default", "appname", "", Settings::AppName, defini);
		//	Ambiesoft::Profile::GetString("default", "defaultpath", "", Settings::DefaultPath, defini);
		//	Ambiesoft::Profile::GetString("default", "section", "", Settings::Section, defini);

		//	return !failed;
		//}

	};


} }

#if _MSC_VER <= 1500
#undef literal
#endif