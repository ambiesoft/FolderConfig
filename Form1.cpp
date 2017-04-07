#include "stdafx.h"
#include "Form1.h"
#include "Settings.h"

namespace Ambiesoft { namespace FolderConfig {
	System::Void Form1::Form1_Load(System::Object^  sender, System::EventArgs^  e) 
	{
		assert(!String::IsNullOrEmpty(Settings::Section));
		try
		{
			int pathtype = 0;
			Profile::GetInt(Settings::Section, "PathType", -1, pathtype, Settings::iniFileName);
			if(pathtype==-1)
			{
				pathtype=0;
			}

			String^ uspath;
			if(Profile::GetString(Settings::Section, "folder", String::Empty, uspath, Settings::iniFileName))
				folbrow->SelectedPath = uspath;

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

				failed |= !Profile::WriteInt(Settings::Section, "PathType", curc_, Settings::iniFileName);
				if(curc_==3)
					failed |= !Profile::WriteString(Settings::Section, "folder", textFolder->Text, Settings::iniFileName);

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

			failed |= !Profile::WriteString(Settings::Section, "PathType", nullptr, Settings::iniFileName);
			failed |= !Profile::WriteString(Settings::Section, "folder", nullptr, Settings::iniFileName);

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