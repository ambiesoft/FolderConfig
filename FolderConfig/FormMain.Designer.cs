
namespace Ambiesoft.FolderConfig
{
    partial class FormMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.radioRoaming = new System.Windows.Forms.RadioButton();
            this.radioUnderThis = new System.Windows.Forms.RadioButton();
            this.radioLocal = new System.Windows.Forms.RadioButton();
            this.radioUserDefine = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.textFolder = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.labelAttention = new System.Windows.Forms.Label();
            this.folbrow = new System.Windows.Forms.FolderBrowserDialog();
            this.btnReverToDefault = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            resources.ApplyResources(this.btnCancel, "btnCancel");
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // btnOK
            // 
            resources.ApplyResources(this.btnOK, "btnOK");
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Name = "btnOK";
            this.btnOK.UseVisualStyleBackColor = true;
            // 
            // radioRoaming
            // 
            resources.ApplyResources(this.radioRoaming, "radioRoaming");
            this.radioRoaming.Name = "radioRoaming";
            this.radioRoaming.TabStop = true;
            this.radioRoaming.UseVisualStyleBackColor = true;
            this.radioRoaming.CheckedChanged += new System.EventHandler(this.radio_CheckedChanged);
            // 
            // radioUnderThis
            // 
            resources.ApplyResources(this.radioUnderThis, "radioUnderThis");
            this.radioUnderThis.Name = "radioUnderThis";
            this.radioUnderThis.TabStop = true;
            this.radioUnderThis.UseVisualStyleBackColor = true;
            this.radioUnderThis.CheckedChanged += new System.EventHandler(this.radio_CheckedChanged);
            // 
            // radioLocal
            // 
            resources.ApplyResources(this.radioLocal, "radioLocal");
            this.radioLocal.Name = "radioLocal";
            this.radioLocal.TabStop = true;
            this.radioLocal.Tag = 1;
            this.radioLocal.UseVisualStyleBackColor = true;
            this.radioLocal.CheckedChanged += new System.EventHandler(this.radio_CheckedChanged);
            // 
            // radioUserDefine
            // 
            resources.ApplyResources(this.radioUserDefine, "radioUserDefine");
            this.radioUserDefine.Name = "radioUserDefine";
            this.radioUserDefine.TabStop = true;
            this.radioUserDefine.UseVisualStyleBackColor = true;
            this.radioUserDefine.CheckedChanged += new System.EventHandler(this.radio_CheckedChanged);
            // 
            // groupBox1
            // 
            resources.ApplyResources(this.groupBox1, "groupBox1");
            this.groupBox1.Controls.Add(this.btnBrowse);
            this.groupBox1.Controls.Add(this.radioRoaming);
            this.groupBox1.Controls.Add(this.radioUnderThis);
            this.groupBox1.Controls.Add(this.radioLocal);
            this.groupBox1.Controls.Add(this.radioUserDefine);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.TabStop = false;
            // 
            // btnBrowse
            // 
            resources.ApplyResources(this.btnBrowse, "btnBrowse");
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.UseVisualStyleBackColor = true;
            this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
            // 
            // textFolder
            // 
            resources.ApplyResources(this.textFolder, "textFolder");
            this.textFolder.Name = "textFolder";
            this.textFolder.ReadOnly = true;
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
            // 
            // labelAttention
            // 
            resources.ApplyResources(this.labelAttention, "labelAttention");
            this.labelAttention.Name = "labelAttention";
            // 
            // folbrow
            // 
            resources.ApplyResources(this.folbrow, "folbrow");
            // 
            // btnReverToDefault
            // 
            resources.ApplyResources(this.btnReverToDefault, "btnReverToDefault");
            this.btnReverToDefault.Name = "btnReverToDefault";
            this.btnReverToDefault.UseVisualStyleBackColor = true;
            this.btnReverToDefault.Click += new System.EventHandler(this.btnReverToDefault_Click);
            // 
            // FormMain
            // 
            this.AcceptButton = this.btnOK;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.CancelButton = this.btnCancel;
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.textFolder);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.labelAttention);
            this.Controls.Add(this.btnReverToDefault);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormMain";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormMain_FormClosing);
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.RadioButton radioRoaming;
        private System.Windows.Forms.RadioButton radioUnderThis;
        private System.Windows.Forms.RadioButton radioLocal;
        private System.Windows.Forms.RadioButton radioUserDefine;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.TextBox textFolder;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label labelAttention;
        private System.Windows.Forms.FolderBrowserDialog folbrow;
        private System.Windows.Forms.Button btnReverToDefault;
    }
}

