#pragma once


namespace Ambiesoft { namespace FolderConfig {

	using namespace System;



	ref class ResUtil {
	public:
		literal String^ RES_FOLDER_SETTINGS_SAVED						= L"Folder settings have been saved.";
		literal String^ RES_FOLDER_SETTINGS_SAVEFAILED					= L"Failed to save settings.";

		static System::Globalization::CultureInfo^ ci_;
		static System::Resources::ResourceManager^ theResource_ = 
			gcnew System::Resources::ResourceManager(
			L"Ambiesoft.FolderConfig.FolderConfigStrings", 
			System::Reflection::Assembly::GetExecutingAssembly());

#ifdef _DEBUG_RESOURCE
		static System::Collections::ArrayList^ da_;

		static void showUnI18Ned(Object^,EventArgs^)
		{
			System::Text::StringBuilder sb;
			sb.AppendLine(L"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII18NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN>>>>>>>>>>>>>>>");
			for each(Object^ o in da_)
			{
				sb.AppendLine(o->ToString());
			}
			sb.AppendLine(L"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII18NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN<<<<<<<<<<<<<<<");
			DTRACE(sb.ToString());
		}
#endif

		static String^ getString(String^ s)
		{
			String^ ret = theResource_->GetString(s, ci_);

#ifdef _DEBUG_RESOURCE
			if (!da_)
			{
				da_ = gcnew System::Collections::ArrayList;
				// System::AppDomain::CurrentDomain->ProcessExit += gcnew EventHandler(gcnew ResUtilHandler(), ResUtilHandler::doWork);
				System::AppDomain::CurrentDomain->ProcessExit += gcnew EventHandler(&ResUtil::showUnI18Ned);
			}

			if ( String::IsNullOrEmpty(ret) && !da_->Contains(s) )
			{
				DTRACE(L"\"" + s + "\" is not I18Ned.");
				da_->Add(s);
			}
#endif
			return String::IsNullOrEmpty(ret) ? s : ret;
		}

		static void setCultureInfo(System::Globalization::CultureInfo^ ci)
		{
			ci_ = ci;
		}
	};



} }