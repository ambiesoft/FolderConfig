#include "stdafx.h"
#include "FormMain.h"

namespace Ambiesoft { namespace FolderConfig {
	void FormMain::InitializeComponent(void)
	{
		System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormMain::typeid));
		this->btnOK = (gcnew System::Windows::Forms::Button());
		this->btnCancel = (gcnew System::Windows::Forms::Button());
		this->radioUnderThis = (gcnew System::Windows::Forms::RadioButton());
		this->radioLocal = (gcnew System::Windows::Forms::RadioButton());
		this->radioUserDefine = (gcnew System::Windows::Forms::RadioButton());
		this->radioRoaming = (gcnew System::Windows::Forms::RadioButton());
		this->label2 = (gcnew System::Windows::Forms::Label());
		this->textFolder = (gcnew System::Windows::Forms::TextBox());
		this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
		this->btnBrowse = (gcnew System::Windows::Forms::Button());
		this->labelAttention = (gcnew System::Windows::Forms::Label());
		this->folbrow = (gcnew System::Windows::Forms::FolderBrowserDialog());
		this->btnReverToDefault = (gcnew System::Windows::Forms::Button());
		this->groupBox1->SuspendLayout();
		this->SuspendLayout();
		// 
		// btnOK
		// 
		this->btnOK->AccessibleDescription = nullptr;
		this->btnOK->AccessibleName = nullptr;
		resources->ApplyResources(this->btnOK, L"btnOK");
		this->btnOK->BackgroundImage = nullptr;
		this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
		this->btnOK->Font = nullptr;
		this->btnOK->Name = L"btnOK";
		this->btnOK->UseVisualStyleBackColor = true;
		// 
		// btnCancel
		// 
		this->btnCancel->AccessibleDescription = nullptr;
		this->btnCancel->AccessibleName = nullptr;
		resources->ApplyResources(this->btnCancel, L"btnCancel");
		this->btnCancel->BackgroundImage = nullptr;
		this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
		this->btnCancel->Font = nullptr;
		this->btnCancel->Name = L"btnCancel";
		this->btnCancel->UseVisualStyleBackColor = true;
		// 
		// radioUnderThis
		// 
		this->radioUnderThis->AccessibleDescription = nullptr;
		this->radioUnderThis->AccessibleName = nullptr;
		resources->ApplyResources(this->radioUnderThis, L"radioUnderThis");
		this->radioUnderThis->BackgroundImage = nullptr;
		this->radioUnderThis->Font = nullptr;
		this->radioUnderThis->Name = L"radioUnderThis";
		this->radioUnderThis->TabStop = true;
		this->radioUnderThis->UseVisualStyleBackColor = true;
		this->radioUnderThis->CheckedChanged += gcnew System::EventHandler(this, &FormMain::radio_CheckedChanged);
		// 
		// radioLocal
		// 
		this->radioLocal->AccessibleDescription = nullptr;
		this->radioLocal->AccessibleName = nullptr;
		resources->ApplyResources(this->radioLocal, L"radioLocal");
		this->radioLocal->BackgroundImage = nullptr;
		this->radioLocal->Font = nullptr;
		this->radioLocal->Name = L"radioLocal";
		this->radioLocal->TabStop = true;
		this->radioLocal->Tag = 1;
		this->radioLocal->UseVisualStyleBackColor = true;
		this->radioLocal->CheckedChanged += gcnew System::EventHandler(this, &FormMain::radio_CheckedChanged);
		// 
		// radioUserDefine
		// 
		this->radioUserDefine->AccessibleDescription = nullptr;
		this->radioUserDefine->AccessibleName = nullptr;
		resources->ApplyResources(this->radioUserDefine, L"radioUserDefine");
		this->radioUserDefine->BackgroundImage = nullptr;
		this->radioUserDefine->Font = nullptr;
		this->radioUserDefine->Name = L"radioUserDefine";
		this->radioUserDefine->TabStop = true;
		this->radioUserDefine->UseVisualStyleBackColor = true;
		this->radioUserDefine->CheckedChanged += gcnew System::EventHandler(this, &FormMain::radio_CheckedChanged);
		// 
		// radioRoaming
		// 
		this->radioRoaming->AccessibleDescription = nullptr;
		this->radioRoaming->AccessibleName = nullptr;
		resources->ApplyResources(this->radioRoaming, L"radioRoaming");
		this->radioRoaming->BackgroundImage = nullptr;
		this->radioRoaming->Font = nullptr;
		this->radioRoaming->Name = L"radioRoaming";
		this->radioRoaming->TabStop = true;
		this->radioRoaming->UseVisualStyleBackColor = true;
		this->radioRoaming->CheckedChanged += gcnew System::EventHandler(this, &FormMain::radio_CheckedChanged);
		// 
		// label2
		// 
		this->label2->AccessibleDescription = nullptr;
		this->label2->AccessibleName = nullptr;
		resources->ApplyResources(this->label2, L"label2");
		this->label2->Font = nullptr;
		this->label2->Name = L"label2";
		// 
		// textFolder
		// 
		this->textFolder->AccessibleDescription = nullptr;
		this->textFolder->AccessibleName = nullptr;
		resources->ApplyResources(this->textFolder, L"textFolder");
		this->textFolder->BackgroundImage = nullptr;
		this->textFolder->Font = nullptr;
		this->textFolder->Name = L"textFolder";
		this->textFolder->ReadOnly = true;
		// 
		// groupBox1
		// 
		this->groupBox1->AccessibleDescription = nullptr;
		this->groupBox1->AccessibleName = nullptr;
		resources->ApplyResources(this->groupBox1, L"groupBox1");
		this->groupBox1->BackgroundImage = nullptr;
		this->groupBox1->Controls->Add(this->btnBrowse);
		this->groupBox1->Controls->Add(this->radioRoaming);
		this->groupBox1->Controls->Add(this->radioUnderThis);
		this->groupBox1->Controls->Add(this->radioLocal);
		this->groupBox1->Controls->Add(this->radioUserDefine);
		this->groupBox1->Font = nullptr;
		this->groupBox1->Name = L"groupBox1";
		this->groupBox1->TabStop = false;
		// 
		// btnBrowse
		// 
		this->btnBrowse->AccessibleDescription = nullptr;
		this->btnBrowse->AccessibleName = nullptr;
		resources->ApplyResources(this->btnBrowse, L"btnBrowse");
		this->btnBrowse->BackgroundImage = nullptr;
		this->btnBrowse->Font = nullptr;
		this->btnBrowse->Name = L"btnBrowse";
		this->btnBrowse->UseVisualStyleBackColor = true;
		this->btnBrowse->Click += gcnew System::EventHandler(this, &FormMain::btnBrowse_Click);
		// 
		// labelAttention
		// 
		this->labelAttention->AccessibleDescription = nullptr;
		this->labelAttention->AccessibleName = nullptr;
		resources->ApplyResources(this->labelAttention, L"labelAttention");
		this->labelAttention->Font = nullptr;
		this->labelAttention->Name = L"labelAttention";
		// 
		// folbrow
		// 
		resources->ApplyResources(this->folbrow, L"folbrow");
		// 
		// btnReverToDefault
		// 
		this->btnReverToDefault->AccessibleDescription = nullptr;
		this->btnReverToDefault->AccessibleName = nullptr;
		resources->ApplyResources(this->btnReverToDefault, L"btnReverToDefault");
		this->btnReverToDefault->BackgroundImage = nullptr;
		this->btnReverToDefault->Font = nullptr;
		this->btnReverToDefault->Name = L"btnReverToDefault";
		this->btnReverToDefault->UseVisualStyleBackColor = true;
		this->btnReverToDefault->Click += gcnew System::EventHandler(this, &FormMain::btnReverToDefault_Click);
		// 
		// FormMain
		// 
		this->AcceptButton = this->btnOK;
		this->AccessibleDescription = nullptr;
		this->AccessibleName = nullptr;
		resources->ApplyResources(this, L"$this");
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->BackgroundImage = nullptr;
		this->CancelButton = this->btnCancel;
		this->Controls->Add(this->btnReverToDefault);
		this->Controls->Add(this->groupBox1);
		this->Controls->Add(this->textFolder);
		this->Controls->Add(this->labelAttention);
		this->Controls->Add(this->label2);
		this->Controls->Add(this->btnCancel);
		this->Controls->Add(this->btnOK);
		this->Font = nullptr;
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->Name = L"FormMain";
		this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
		this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormMain::FormMain_FormClosing);
		this->groupBox1->ResumeLayout(false);
		this->groupBox1->PerformLayout();
		this->ResumeLayout(false);
		this->PerformLayout();

	}

} }