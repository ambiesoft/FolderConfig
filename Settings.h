
#if _MSC_VER <= 1500
#define literal static initonly
#endif

namespace Ambiesoft { namespace FolderConfig {

	using namespace System;

	ref class Settings abstract sealed
	{
	private:
		static String^ title_;
		// static String^ iniFileName_;
		static String^ creator_;
		static String^ appName_;
		static String^ section_;
		static String^ defaultUserPath_;
		
		literal String^ DefaultInifileName = L"folder.ini";
		literal String^ DefaultCreator = L"me";
		literal String^ DefaultAppName = L"MyApp";

		static bool initialized_;

	public:
		static bool init();

		static property String^ Title
		{
			String^ get()
			{
				if (String::IsNullOrEmpty(title_))
				{
					return System::Windows::Forms::Application::ProductName;
				}
				return title_;
			}
		}
		static property String^ Section
		{
			String^ get() { return section_; }
		}
		static property String^ InifileName
		{
			String^ get() { return DefaultInifileName; }
		}
		static property String^ Creator
		{
			String^ get() { return creator_; }
		}
		static property String^ AppName
		{
			String^ get() { return appName_; }
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