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
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;

	public ref class FormMain : public System::Windows::Forms::Form
	{
	public:
		literal String^ KEY_PATH_TYPE				= L"PathType";
		literal String^ KEY_FOLDER					= L"folder";
	public:
		FormMain(void)
		{
			InitializeComponent();
		}

	protected:
		~FormMain()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnOK;
	protected: 
	private: System::Windows::Forms::Button^  btnCancel;
	private: System::Windows::Forms::RadioButton^  radioUnderThis;
	private: System::Windows::Forms::RadioButton^  radioLocal;
	private: System::Windows::Forms::RadioButton^  radioUserDefine;




	private: System::Windows::Forms::RadioButton^  radioRoaming;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textFolder;

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Button^  btnBrowse;
	private: System::Windows::Forms::Label^  labelAttention;
	private: System::Windows::Forms::FolderBrowserDialog^  folbrow;

	private: System::Windows::Forms::Button^  btnReverToDefault;


	private:
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void);
#pragma endregion


	private: int curc_;
	//private: String^ APP_KEYNAME_FOLDER;
	//literal String^ AMBIESOFTNAME = L"Ambiesoft";
	//literal String^ DICREGATENAME = L"Dicregate";

	private: System::Void FormMain_Load(System::Object^  sender, System::EventArgs^  e);

	private: System::Void checkedCommon(int i);
	private: System::Void radio_CheckedChanged(System::Object^  sender, System::EventArgs^  e) ;


	private: System::Void btnBrowse_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void FormMain_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) ;

	private: System::Void btnReverToDefault_Click(System::Object^  sender, System::EventArgs^  e);


	};





} }

