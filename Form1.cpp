#include "stdafx.h"
#include "Form1.h"
#include "Settings.h"

namespace Ambiesoft { namespace FolderConfig {
	System::Void Form1::Form1_Load(System::Object^  sender, System::EventArgs^  e) 
	{
		// bool isForSes=false;
		//array<System::String ^> ^args = System::Environment::GetCommandLineArgs();
		//if(args && args->Length > 1)
		//{
		//	for(int i=1 ; i<args->Length; ++i)
		//	{
		//		String^ arg = args[i];
		//		if(arg=="/sm")
		//		{
		//			isForSes=true;
		//			btnReverToDefault->Visible = true;
		//		}
		//		else
		//		{
		//			MessageBox::Show((L"Unknown Option") + L": " + arg,
		//				Application::ProductName,
		//				MessageBoxButtons::OK,
		//				MessageBoxIcon::Error);
		//			Close();
		//			return;
		//		}
		//	}
		//}

		APP_KEYNAME_FOLDER = L"Folder";
		//if(isForSes)
		//{
		//	APP_KEYNAME_FOLDER = L"SessionFolder";
		//	Text=Text + L" - SessionManager";
		//}

		try
		{
			int pathtype = 0;
			Profile::GetInt(APP_KEYNAME_FOLDER, "PathType", -1, pathtype, Settings::iniFileName);
			if(pathtype==-1)
			{
				pathtype=0;
			}

			//if(!isForSes)
			//{
			//	if(pathtype==-1)
			//	{
			//		pathtype=0;
			//	}
			//}
			//else
			//{
			//	if(pathtype==-1)
			//	{ // SessionFolder has no entry
			//		Profile::GetInt(L"Folder", "PathType", 0, pathtype, inipath);
			//	}
			//}




			String^ uspath;
			if(Profile::GetString(APP_KEYNAME_FOLDER, "UserDefinedPath", String::Empty, uspath, Settings::iniFileName))
				folbrow->SelectedPath = uspath;

			//if(isForSes)
			//{
			//	if(String::IsNullOrEmpty(uspath))
			//	{
			//		Profile::GetString(L"Folder", "UserDefinedPath", String::Empty, uspath, inipath);
			//	}
			//}
			//			String^ path;
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
			default:throw gcnew Exception(String::Format(L"{0} has wrong value.", Settings::iniFileName));
			}
			//			textFolder->Text = path;
		}
		catch(Exception^ e)
		{
			MessageBox::Show(L"error : " + e->Message);
			radioUnderThis->Checked = true;
		}

		this->Text = String::IsNullOrEmpty(Settings::Title) ? Application::ProductName : Settings::Title;
	}


	System::Void Form1::btnOK_Click(System::Object^  sender, System::EventArgs^  e) 
	{
	}



	System::Void Form1::checkedCommon(int i)
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
					if(!String::IsNullOrEmpty(Settings::DefaultPath))
						path = System::IO::Path::Combine(path, Settings::DefaultPath);
					// labelAttention->Text = String::Empty;
					labelAttention->Visible = false;
				}
				break;
			case 1:
				{
					path = System::Environment::GetFolderPath(System::Environment::SpecialFolder::LocalApplicationData);
					path = System::IO::Path::Combine(path, Settings::Creator);
					path = System::IO::Path::Combine(path, Settings::AppName);
					// labelAttention->Text = L"username will be replaced by actual username.";
					labelAttention->Visible = true;
				}
				break;
			case 2: 
				{
					path = System::Environment::GetFolderPath(System::Environment::SpecialFolder::ApplicationData);
					path = System::IO::Path::Combine(path, Settings::Creator);
					path = System::IO::Path::Combine(path, Settings::AppName);
					// labelAttention->Text = L"username will be replaced by actual username.";
					labelAttention->Visible = true;
				}
				break;
			case 3: 
				{
					path = folbrow->SelectedPath;
					// labelAttention->Text = String::Empty;
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

	System::Void Form1::radio_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
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
			else
				curc_ = 3;

			checkedCommon(curc_);
		}
	}



	System::Void Form1::btnBrowse_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		folbrow->SelectedPath = textFolder->Text;
		if ( System::Windows::Forms::DialogResult::OK == folbrow->ShowDialog(this) )
			textFolder->Text = folbrow->SelectedPath;
	}



	System::Void Form1::Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
	{
		if ( DialogResult == System::Windows::Forms::DialogResult::OK )
		{
			if ( String::IsNullOrEmpty(textFolder->Text) )
			{
				e->Cancel = true;
				MessageBox::Show(I18N(L"This folder is not available."),
					Settings::Title,
					MessageBoxButtons::OK,
					MessageBoxIcon::Exclamation);
				return;
			}

			bool ok = false;
			try
			{
				bool failed = false;

				failed |= !Profile::WriteInt(APP_KEYNAME_FOLDER, "PathType", curc_, Settings::iniFileName);
				if(curc_==3)
					failed |= !Profile::WriteString(APP_KEYNAME_FOLDER, "UserDefinedPath", textFolder->Text, Settings::iniFileName);

				ok = !failed;
			}
			catch(Exception^)
			{
			}

			if ( !ok )
			{
				MessageBox::Show(this, L"Failed to save settings.", Application::ProductName, MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			else
			{
				MessageBox::Show(this, lblLiteralOKMessage->Text, Application::ProductName, MessageBoxButtons::OK, MessageBoxIcon::Information);
			}
		}
	}


	System::Void Form1::btnReverToDefault_Click(System::Object^  sender, System::EventArgs^  e)
	{
		bool ok = false;
		try
		{
			bool failed = false;

			failed |= !Profile::WriteString(APP_KEYNAME_FOLDER, "PathType", nullptr, Settings::iniFileName);
			failed |= !Profile::WriteString(APP_KEYNAME_FOLDER, "UserDefinedPath", nullptr, Settings::iniFileName);

			ok = !failed;
		}
		catch(Exception^)
		{
		}

		if ( !ok )
		{
			MessageBox::Show(this, L"Failed to save settings.", Application::ProductName, MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		else
		{
			MessageBox::Show(this, lblLiteralOKMessage->Text, Application::ProductName, MessageBoxButtons::OK, MessageBoxIcon::Information);
		}		 
		Close();
	}

} }