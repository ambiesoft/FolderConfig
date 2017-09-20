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

#include "../../lsMisc/BrowseFolder.h"

namespace Ambiesoft { namespace FolderConfig {
	
	//String^ FormMain::getI18NString(String^ sIn)
	//{
	//	String^ ret = theResource_->GetString(sIn);
	//	return String::IsNullOrEmpty(ret) ? sIn : ret;
	//}

	System::Void FormMain::FormMain_Load(System::Object^  sender, System::EventArgs^  e) 
	{
		assert(!String::IsNullOrEmpty(Settings::Section));
		try
		{
			int pathtype = 0;
			Profile::GetInt(Settings::Section, KEY_PATH_TYPE, -1, pathtype, Settings::UserIniFullpath);
			if(pathtype==-1)
			{
				pathtype=Settings::DefaultPathType;
			}

			String^ uspath;
			if (Profile::GetString(Settings::Section, KEY_FOLDER, String::Empty, uspath, Settings::UserIniFullpath))
				folbrow->SelectedPath = uspath;
			else
			{
				if(!String::IsNullOrEmpty(Settings::DefaultPath3))
					folbrow->SelectedPath = Settings::DefaultPath3;
			}

			switch(pathtype)
			{
			case 0: 
				{
					radioUnderThis->Checked = true;
				}
				break;
			case 1:
				{
					radioLocal->Checked = true;
				}
				break;
			case 2: 
				{
					radioRoaming->Checked = true;
				}
				break;
			case 3: 
				{
					radioUserDefine->Checked = true;
				}
				break;
			default:
				throw gcnew Exception(String::Format(L"{0} has wrong value.", Settings::UserIniFullpath));
			}

		}
		catch(Exception^ e)
		{
			MessageBox::Show(L"error : " + e->Message);
			radioUnderThis->Checked = true;
		}


		this->Text = String::IsNullOrEmpty(Settings::Title) ? 
			ProductName + L" | " + Settings::AppName + L" | " + Settings::Section:
		Settings::Title;
	}






	System::Void FormMain::checkedCommon(int i)
	{
		String^ path;
		try
		{
			switch(i)
			{
			case 0: 
				{
					System::IO::FileInfo fi(Application::ExecutablePath);
					System::IO::DirectoryInfo^ di = fi.Directory;
					path = di->FullName;
					if(!String::IsNullOrEmpty(Settings::DefaultPath0))
						path = System::IO::Path::Combine(path, Settings::DefaultPath0);
					labelAttention->Visible = false;
				}
				break;
			case 1:
				{
					path = System::Environment::GetFolderPath(System::Environment::SpecialFolder::LocalApplicationData);
					path = System::IO::Path::Combine(path, Settings::Creator);
					path = System::IO::Path::Combine(path, Settings::AppName);
					labelAttention->Visible = true;
				}
				break;
			case 2: 
				{
					path = System::Environment::GetFolderPath(System::Environment::SpecialFolder::ApplicationData);
					path = System::IO::Path::Combine(path, Settings::Creator);
					path = System::IO::Path::Combine(path, Settings::AppName);
					labelAttention->Visible = true;
				}
				break;
			case 3: 
				{
					path = folbrow->SelectedPath;
					labelAttention->Visible = false;
				}
				break;
			default:
				throw gcnew Exception(L"wrong checked value");
			}
		}
		catch(Exception^ e)
		{
			MessageBox::Show(L"error : " + e->Message);
			radioUnderThis->Checked = true;
		}
		textFolder->Text = path;
		textFolder->ReadOnly  = ( i != 3 );
		btnBrowse->Enabled = (i == 3);
	}

	System::Void FormMain::radio_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		RadioButton^ rb = (RadioButton^)sender;
		if(rb->Checked)
		{
			if ( rb == radioUnderThis )
				curc_ = 0;
			else if ( rb == radioLocal )
				curc_ = 1;
			else if ( rb == radioRoaming )
				curc_ = 2;
			else if( rb == radioUserDefine)
				curc_ = 3;
			else
				DASSERT(false);

			checkedCommon(curc_);
		}
	}



	System::Void FormMain::btnBrowse_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		String^ folder = textFolder->Text;
		if(!browseFolder(this, I18N(L"Select Folder"), folder))
			return;

		textFolder->Text = folder;
	}



	System::Void FormMain::FormMain_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
	{
		if ( DialogResult == System::Windows::Forms::DialogResult::OK )
		{
			if ( String::IsNullOrEmpty(textFolder->Text) )
			{
				e->Cancel = true;
				MessageBox::Show(I18N(L"Folder must not be empty."),
					Settings::Title,
					MessageBoxButtons::OK,
					MessageBoxIcon::Exclamation);
				return;
			}

			bool ok = false;
			try
			{
				bool failed = false;

				failed |= !Profile::WriteInt(Settings::Section, KEY_PATH_TYPE, curc_, Settings::UserIniFullpath);
				if(curc_==3)
					failed |= !Profile::WriteString(Settings::Section, KEY_FOLDER, textFolder->Text, Settings::UserIniFullpath);

				ok = !failed;
			}
			catch(Exception^)
			{
			}

			if ( !ok )
			{
				MessageBox::Show(this,
					I18N(ResUtil::RES_FOLDER_SETTINGS_SAVEFAILED), 
					ProductName,
					MessageBoxButtons::OK,
					MessageBoxIcon::Error);
			}
			else
			{
				MessageBox::Show(this,
					I18N(ResUtil::RES_FOLDER_SETTINGS_SAVED),
					ProductName,
					MessageBoxButtons::OK,
					MessageBoxIcon::Information);
			}
		}
	}


	System::Void FormMain::btnReverToDefault_Click(System::Object^  sender, System::EventArgs^  e)
	{
		bool ok = false;
		try
		{
			bool failed = false;

			failed |= !Profile::WriteString(Settings::Section, KEY_PATH_TYPE, nullptr, Settings::UserIniFullpath);
			failed |= !Profile::WriteString(Settings::Section, KEY_FOLDER, nullptr, Settings::UserIniFullpath);

			ok = !failed;
		}
		catch(Exception^)
		{
		}

		if ( !ok )
		{
			MessageBox::Show(this, 
				I18N(ResUtil:: RES_FOLDER_SETTINGS_SAVEFAILED),
				ProductName,
				MessageBoxButtons::OK,
				MessageBoxIcon::Error);
		}
		else
		{
			MessageBox::Show(this,
				I18N(ResUtil:: RES_FOLDER_SETTINGS_SAVED),
				ProductName,
				MessageBoxButtons::OK,
				MessageBoxIcon::Information);
		}		 
		Close();
	}

} }