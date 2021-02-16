using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ambiesoft.FolderConfig
{
    abstract class Settings 
    {
		public const string ProductName				= "FolderConfig";

	private static string title_;

private static string creator_;
private static string appName_;
		static string section_;
		
		private const string section_default_		= "Main";
		
		private static int defaultpathtype_;

		private static string defaultpath0_;
		private static string defaultpath3_;


		private const string userInifileName_ = "folder.ini";

		private static bool initialized_;


		public static bool init();

		static property String^ Title
		{
			String^ get()
		{
			//if (String::IsNullOrEmpty(title_))
			//{
			//	return System::Windows::Forms::ProductName;
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

		static property int DefaultPathType
{
			int get() { return defaultpathtype_; }
	}
		static property String^ DefaultPath0
		{
			String^ get();
		}
		static property String^ DefaultPath3
		{
			String^ get() { return defaultpath3_; };
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

    }
}
