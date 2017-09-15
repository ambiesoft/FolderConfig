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