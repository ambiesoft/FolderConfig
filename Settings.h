
namespace Ambiesoft { namespace FolderConfig {

	using namespace System;

	ref class Settings
	{
	public:
		static String^ title;
		static String^ iniFileName;
		static String^ Creator;
		static String^ AppName;
		static String^ DefaultPath;
		
		static property String^ Title
		{
			String^ get()
			{
				if(String::IsNullOrEmpty(title))
				{
					return System::Windows::Forms::Application::ProductName;
				}
				return title;
			}
		}
		static String^ Section;

		
		static public bool initDefault()
		{
			String^ dir = System::IO::Directory::GetParent(System::Windows::Forms::Application::ExecutablePath)->FullName;
			String^ definifile = System::IO::Path::Combine(dir, "folderconfig.ini");

			bool failed = false;
			Ambiesoft::HashIni^ defini = Ambiesoft::Profile::ReadAll(definifile);
			Ambiesoft::Profile::GetString("default", "title", "", Settings::title, defini);
			Ambiesoft::Profile::GetString("default", "inifile", "", Settings::iniFileName, defini);
			Ambiesoft::Profile::GetString("default", "creator", "", Settings::Creator, defini);
			Ambiesoft::Profile::GetString("default", "appname", "", Settings::AppName, defini);
			Ambiesoft::Profile::GetString("default", "defaultpath", "", Settings::DefaultPath, defini);
			Ambiesoft::Profile::GetString("default", "section", "", Settings::Section, defini);

			return !failed;
		}

	};


} }