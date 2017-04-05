
namespace Ambiesoft { namespace FolderConfig {

	using namespace System;

	ref class Settings
	{
	public:
		static String^ title;
		static String^ iniFileName;
		static property String^ Creator;
		static property String^ AppName;
		static property String^ DefaultPath;
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

		

	};


} }