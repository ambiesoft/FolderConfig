using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ambiesoft.FolderConfig
{
	public partial class FormMain : Form
	{
		const string KEY_PATH_TYPE = "PathType";
		const string KEY_FOLDER = "folder";

		int curc_;

		public FormMain()
		{
			InitializeComponent();
		}

		private void FormMain_Load(object sender, EventArgs e)
		{
			Debug.Assert(!string.IsNullOrEmpty(Settings.Section));
			try
			{
				int pathtype = 0;
				Profile.GetInt(Settings.Section, KEY_PATH_TYPE, -1, out pathtype, Settings.UserIniFullpath);
				if (pathtype == -1)
				{
					pathtype = Settings.DefaultPathType;
				}

				string uspath;
				if (Profile.GetString(Settings.Section, KEY_FOLDER, string.Empty, out uspath, Settings.UserIniFullpath))
					folbrow.SelectedPath = uspath;
				else
				{
					if (!string.IsNullOrEmpty(Settings.DefaultPath3))
						folbrow.SelectedPath = Settings.DefaultPath3;
				}

				switch (pathtype)
				{
					case 0:
						{
							radioUnderThis.Checked = true;
						}
						break;
					case 1:
						{
							radioLocal.Checked = true;
						}
						break;
					case 2:
						{
							radioRoaming.Checked = true;
						}
						break;
					case 3:
						{
							radioUserDefine.Checked = true;
						}
						break;
					default:
						throw new Exception(string.Format(Properties.Resources._0_HAS_WRONG_VALUE,
							Settings.UserIniFullpath));
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show("error : " + ex.Message);
				radioUnderThis.Checked = true;
			}


			Text = String.IsNullOrEmpty(Settings.Title) ?
				ProductName + " | " + Settings.AppName + " | " + Settings.Section :
				Settings.Title;
		}

		void checkedCommon(int i)
		{
			string path = null;
			try
			{
				switch (i)
				{
					case 0:
						{
							FileInfo fi = new FileInfo(Application.ExecutablePath);
							DirectoryInfo di = fi.Directory;
							path = di.FullName;
							if (!String.IsNullOrEmpty(Settings.DefaultPath0))
								path = System.IO.Path.Combine(path, Settings.DefaultPath0);
							labelAttention.Visible = false;
						}
						break;
					case 1:
						{
							path = System.Environment.GetFolderPath(System.Environment.SpecialFolder.LocalApplicationData);
							path = System.IO.Path.Combine(path, Settings.Creator);
							path = System.IO.Path.Combine(path, Settings.AppName);
							labelAttention.Visible = true;
						}
						break;
					case 2:
						{
							path = System.Environment.GetFolderPath(System.Environment.SpecialFolder.ApplicationData);
							path = System.IO.Path.Combine(path, Settings.Creator);
							path = System.IO.Path.Combine(path, Settings.AppName);
							labelAttention.Visible = true;
						}
						break;
					case 3:
						{
							path = folbrow.SelectedPath;
							labelAttention.Visible = false;
						}
						break;
					default:
						throw new Exception("wrong checked value");
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show("error : " + ex.Message);
				radioUnderThis.Checked = true;
			}
			textFolder.Text = path;
			textFolder.ReadOnly = (i != 3);
			btnBrowse.Enabled = (i == 3);
		}

		private void radio_CheckedChanged(object sender, EventArgs e)
		{
			RadioButton rb = (RadioButton)sender;
			if (rb.Checked)
			{
				if (rb == radioUnderThis)
					curc_ = 0;
				else if (rb == radioLocal)
					curc_ = 1;
				else if (rb == radioRoaming)
					curc_ = 2;
				else if (rb == radioUserDefine)
					curc_ = 3;
				else
					Debug.Assert(false);

				checkedCommon(curc_);
			}
		}

		private void btnBrowse_Click(object sender, EventArgs e)
		{
			string folderPath = "";
			FolderBrowserDialog folderBrowserDialog1 = new FolderBrowserDialog();
			if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
			{
				folderPath = folderBrowserDialog1.SelectedPath;
			}

			textFolder.Text = folderPath;
		}

		private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
		{
			if (DialogResult == System.Windows.Forms.DialogResult.OK)
			{
				if (String.IsNullOrEmpty(textFolder.Text))
				{
					e.Cancel = true;
					MessageBox.Show(Properties.Resources.FOLDER_MUST_NOT_BE_EMPTY,
						Settings.Title,
						MessageBoxButtons.OK,
						MessageBoxIcon.Exclamation);
					return;
				}

				bool ok = false;
				try
				{
					bool failed = false;

					failed |= !Profile.WriteInt(Settings.Section, KEY_PATH_TYPE, curc_, Settings.UserIniFullpath);
					if (curc_ == 3)
						failed |= !Profile.WriteString(Settings.Section, KEY_FOLDER, textFolder.Text, Settings.UserIniFullpath);

					ok = !failed;
				}
				catch (Exception)
				{
				}

				if (!ok)
				{
					MessageBox.Show(
						this,
						Properties.Resources.FOLDER_SETTINGS_SAVEFAILED,
						ProductName,
						MessageBoxButtons.OK,
						MessageBoxIcon.Error);
				}
				else
				{
					MessageBox.Show(
						this,
						Properties.Resources.FOLDER_SETTINGS_SAVEOK,
						ProductName,
						MessageBoxButtons.OK,
						MessageBoxIcon.Information);
				}
			}

		}

		private void btnReverToDefault_Click(object sender, EventArgs e)
		{
			bool ok = false;
			try
			{
				bool failed = false;

				failed |= !Profile.WriteString(Settings.Section, KEY_PATH_TYPE, null, Settings.UserIniFullpath);
				failed |= !Profile.WriteString(Settings.Section, KEY_FOLDER, null, Settings.UserIniFullpath);

				ok = !failed;
			}
			catch (Exception)
			{
			}

			if (!ok)
			{
				MessageBox.Show(this,
					Properties.Resources.FOLDER_SETTINGS_SAVEFAILED,
					ProductName,
					MessageBoxButtons.OK,
					MessageBoxIcon.Error);
			}
			else
			{
				MessageBox.Show(this,
					Properties.Resources.FOLDER_SETTINGS_SAVEOK,
					ProductName,
					MessageBoxButtons.OK,
					MessageBoxIcon.Information);
			}
			Close();
		}
    }
}
