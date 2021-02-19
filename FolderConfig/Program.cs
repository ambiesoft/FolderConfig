using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Windows.Forms;

namespace Ambiesoft.FolderConfig
{
	class PowWrite
	{
		const string CUSTOMER_TID_APP = "TestWrite";
		const string CUSTOMER_TID_KEY = "TID";

		public static bool IsAdmin()
		{
			try
			{
				System.Security.Principal.WindowsIdentity wi = System.Security.Principal.WindowsIdentity.GetCurrent();
				System.Security.Principal.WindowsPrincipal wp = new System.Security.Principal.WindowsPrincipal(wi);
				return wp.IsInRole(System.Security.Principal.WindowsBuiltInRole.Administrator);
			}
			catch (System.Exception)
			{
			}
			return false;
		}
		public static bool do_c_write(string iniFileName)
		{
			int val = System.Environment.TickCount;

			Ambiesoft.HashIni hi = Ambiesoft.Profile.ReadAll(iniFileName);
			Ambiesoft.Profile.WriteInt(
				CUSTOMER_TID_APP,
				CUSTOMER_TID_KEY,
				val,
				hi);
			Ambiesoft.Profile.WriteAll(hi, iniFileName);



			int readv;
			Ambiesoft.Profile.GetInt(
				CUSTOMER_TID_APP,
				CUSTOMER_TID_KEY,
				-1,
				out readv,
				iniFileName);

			return val == readv;
		}

		public static string getArgForCreate(string[] args)
		{
			System.Text.StringBuilder sb = new System.Text.StringBuilder();
			foreach (string s in args)
			{
				if (s.IndexOf(' ') > 0)
					sb.Append("\"" + s + "\"");
				else
					sb.Append(s);

				sb.Append(" ");
			}
			return sb.ToString().TrimEnd();
		}
	}

	// up to 15 byte
	enum ErrorReturnValue
	{
		ErrorReturn_NoError = 0,
		ErrorReturn_SettingsInitFailed,
		ErrorReturn_ParseFailed,
		ErrorReturn_RunAsFailed,
	};


	public static class Program
    {
		static int MakeRetval(bool success, ErrorReturnValue errorCode, bool okcancel)
		{
			return (int)(
					((uint)(success ? 0 : 1)) << 31 |
					((uint)1) << 29 | // customer code (0x20000000)
					((uint)errorCode) << 1 | // errorCode
					((uint)(okcancel ? 1 : 0))
			);
		}

		public static int libmain(string[] args)
		{
			if (!Settings.init(args))
			{
				return MakeRetval(false, ErrorReturnValue.ErrorReturn_SettingsInitFailed, false);
			}

			if (!PowWrite.IsAdmin() && !PowWrite.do_c_write(Settings.UserIniFullpath))
			{
				// could not write to inifile, launch me in higher priviledge
				try
				{
					ProcessStartInfo psi = new ProcessStartInfo();
					psi.UseShellExecute = true;
					psi.WorkingDirectory = System.Environment.CurrentDirectory;
					psi.FileName = Application.ExecutablePath;
					psi.Arguments = PowWrite.getArgForCreate(args);
					psi.Verb = "runas";
					Process pro = Process.Start(psi);
					pro.WaitForExit();

					Debug.WriteLine(pro.ExitCode);
					return pro.ExitCode;
				}
				catch (Exception ex)
				{
					MessageBox.Show(
						ex.Message,
						Settings.Title,
						MessageBoxButtons.OK,
						MessageBoxIcon.Error);

					return MakeRetval(false, ErrorReturnValue.ErrorReturn_RunAsFailed, false);
				}
			}


			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);

			using (FormMain dlg = new FormMain())
			{
				DialogResult dr = dlg.ShowDialog();

				return MakeRetval(true, ErrorReturnValue.ErrorReturn_NoError,
					dr != DialogResult.Cancel);
			}
		}
		
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
			[STAThread]
        public static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new FormMain());
        }
    }
}
