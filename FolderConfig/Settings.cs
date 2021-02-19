using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using NDesk.Options;

namespace Ambiesoft.FolderConfig
{
	internal abstract class Settings
	{
		public const string ProductName = "FolderConfig";

		private static string _title;

		private static string creator_;
		private static string appName_;
		static string section_;

		private const string section_default_ = "Main";

		private static int defaultpathtype_;

		private static string defaultpath0_;
		private static string defaultpath3_;


		private const string userInifileName_ = "folder.ini";

		// private static bool initialized_;

		static string getHelpMessage()
		{
			StringBuilder sb = new StringBuilder();
			sb.AppendLine(Properties.Resources.USAGE + ":");
			sb.AppendLine("FolderConfig.exe [/section Section] [/title Title] [/inifile Inifile] /defaultpathtype Defaultpathtype [/defaultpath0 Defaultpath0] [/defaultpath3 Defaultpath3] [/creator Creator] [/appname Appname] [/culture Culture]");
			sb.AppendLine();

			sb.AppendLine(Properties.Resources.HELP_SECTION_NAME_OF_CONFIG);

			sb.AppendLine(Properties.Resources.HELP_TITLE);
			sb.AppendLine(Properties.Resources.HELP_INIFILE);

			sb.AppendLine(Properties.Resources.HELP_DEFAULT_FILE);
			sb.AppendLine(Properties.Resources.HELP_DEFAULT_PATH0);
			sb.AppendLine(Properties.Resources.HELP_DEFAULT_PATH3);
			sb.AppendLine(Properties.Resources.HELP_CREATOR);
			sb.AppendLine(Properties.Resources.HELP_APPNAME);

			sb.AppendLine(Properties.Resources.HELP_CULTURE);

			return sb.ToString();
		}

		static void ShowError(string s)
		{
			MessageBox.Show(s + "\r\n\r\n" + getHelpMessage(),
				Settings.ProductName,
				MessageBoxButtons.OK,
				MessageBoxIcon.Error);
		}
		static void Alert(string msg)
		{
			MessageBox.Show(msg,
				Settings.ProductName,
				MessageBoxButtons.OK,
				MessageBoxIcon.Warning);
		}
		static void Alert(Exception ex)
		{
			Alert(ex.Message);
		}

		static char[] GetRelativePathInamableChars()
		{
			return ":*?\"<>|".ToCharArray();
		}
		static bool IsRelativePathNamble(string name)
		{
			if (string.IsNullOrEmpty(name))
				return false;

			return name.IndexOfAny(GetRelativePathInamableChars()) >= 0;
		}

		static char[] GetFileInamableChars()
		{
			return "/\\:*?\"<>|".ToCharArray();
		}
		static bool IsFileNamble(string name)
		{
			if (string.IsNullOrEmpty(name))
				return false;

			return name.IndexOfAny(GetFileInamableChars()) >= 0;
		}
		public static bool init(string[] args)
        {
			string opTitle = null;
			string opDefaultPathType = null;
			string opDefaultPath0 = null;
			string opDefaultPath3 = null;
			string opCreator = null;
			string opAppName = null;
			string opSection = null;
			string opCulture = null;
			bool opHelp = false;

			string errorOp = null;
			var p = new OptionSet() {
					{
						"title",
						"title",
						v => { 
							if(!string.IsNullOrEmpty(opTitle))
                            {
								errorOp="title";
                            }
							opTitle = v; 
						}
					},
					{
						"defaultpathtype",
						"defaultpathtype",
						v => { 
							if(!string.IsNullOrEmpty(opDefaultPathType))
                            {
								errorOp="defaultpathtype";
                            }
							opDefaultPathType=v; 
						}
					},
					{
						"defaultpath0",
						"defaultpath0",
						v => { 
							if(!string.IsNullOrEmpty(opDefaultPath0))
                            {
								errorOp="defaultpath0";
                            }
							opDefaultPath0=v; 
						}
					},
					{
						"defaultpath3",
						"defaultpath3",
						v => {
							if(!string.IsNullOrEmpty(opDefaultPath3))
							{
								errorOp="defaultpath3";
							}
							opDefaultPath3=v;
						}
					},
					{
						"creator",
						"creator",
						v => {
							if(!string.IsNullOrEmpty(opCreator))
							{
								errorOp="creator";
							}
							opCreator=v;
						}
					},
					{
						"appname",
						"appname",
						v => {
							if(!string.IsNullOrEmpty(opAppName))
							{
								errorOp="appname";
							}
							opAppName=v;
						}
					},
					{
						"section",
						"section",
						v => {
							if(!string.IsNullOrEmpty(opSection))
							{
								errorOp="section";
							}
							opSection=v;
						}
					},
					{
						"culture",
						"culture",
						v => {
							if(!string.IsNullOrEmpty(opCulture))
							{
								errorOp="culture";
							}
							opCulture=v;
						}
					},
					{
						"h",
						"help",
						v => {
							opHelp=true;
						}
					},
					{
						"?",
						"help",
						v => {
							opHelp=true;
						}
					},




			};

			List<string> extra = p.Parse(args);

			if(errorOp != null)
            {
				ShowError(string.Format(Properties.Resources._0_IS_SET_TWICE, errorOp));
				return false;
			}

			string culture = null;

			string inipath = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath),
				ProductName + ".ini");
			
			HashIni ini = Profile.ReadAll(inipath);

			if (opSection != null)
			{
				section_ = opSection;
			}
			if (string.IsNullOrEmpty(Section))
			{
				section_ = section_default_;
			}


			Profile.GetString(Section, "title", null, out _title, ini);
			Profile.GetString(Section, "appname", null, out appName_, ini);
			Profile.GetString(Section, "creator", null, out creator_, ini);
			Profile.GetInt(Section, "defaultpathtype", -1, out defaultpathtype_, ini);
			Profile.GetString(Section, "defaultpath0", null, out defaultpath0_, ini);
			Profile.GetString(Section, "defaultpath3", null, out defaultpath3_, ini);
			Profile.GetString(Section, "culture", null, out culture, ini);
#if DEBUG
			if (!String.IsNullOrEmpty(_title))
				System.Diagnostics.Debug.Print("title is \"{0}\" (from \"{1}\")", _title, inipath);
			if (!String.IsNullOrEmpty(appName_))
				System.Diagnostics.Debug.Print("Appname is \"{0}\" (from \"{1}\")", appName_, inipath);
			if (!String.IsNullOrEmpty(creator_))
				System.Diagnostics.Debug.Print("Creator is \"{0}\" (from \"{1}\")", creator_, inipath);
			if (defaultpathtype_ >= 0)
				System.Diagnostics.Debug.Print("defaultpathtype is \"{0}\" (from \"{1}\")", defaultpathtype_, inipath);
			if (!String.IsNullOrEmpty(defaultpath0_))
				System.Diagnostics.Debug.Print("defaultpath0_ is \"{0}\" (from \"{1}\")", defaultpath0_, inipath);
			if (!String.IsNullOrEmpty(defaultpath3_))
				System.Diagnostics.Debug.Print("defaultpath3_ is \"{0}\" (from \"{1}\")", defaultpath3_, inipath);
			if (!String.IsNullOrEmpty(culture))
				System.Diagnostics.Debug.Print("culture is \"{0}\" (from \"{1}\")", culture, inipath);
#endif


			// First, set culture for I18N
			if (opCulture!=null)
			{
				culture = opCulture;
			}
			if (!string.IsNullOrEmpty(culture))
			{
				try
				{
					var ci = new System.Globalization.CultureInfo(culture);
					System.Threading.Thread.CurrentThread.CurrentCulture = ci;
					System.Threading.Thread.CurrentThread.CurrentUICulture = ci;
				}
				catch (Exception ex)
				{
					Alert(ex);
				}
			}

			if (opHelp)
			{
				MessageBox.Show(getHelpMessage(),
					ProductName,
					MessageBoxButtons.OK,
					MessageBoxIcon.Information
					);
				return false;
			}

			if (opTitle != null)
				_title = opTitle;

			if (opDefaultPathType != null)
			{
				if (opDefaultPathType != "0" &&
					opDefaultPathType != "1" &&
					opDefaultPathType != "2" &&
					opDefaultPathType != "3")
				{
					ShowError(Properties.Resources.DEFAULTTYPE_MUST_BE_0123);
					return false;
				}
				int.TryParse(opDefaultPathType, out defaultpathtype_);
			}

			if (opDefaultPath0 != null)
			{
				if (!IsRelativePathNamble(opDefaultPath0))
				{
					ShowError(
						string.Format(Properties.Resources._0_MUST_NOT_INCLUDE_1,
						"defaultpath",
						GetRelativePathInamableChars().ToString()));
					return false;
				}
				defaultpath0_ = opDefaultPath0;
			}

			if (opDefaultPath3 != null)
				defaultpath3_ = opDefaultPath3;

			if (opCreator!=null)
			{
				if (!IsFileNamble(opCreator))
				{
					ShowError(
						string.Format(Properties.Resources._0_MUST_NOT_INCLUDE_1,
						"creator",
						GetFileInamableChars().ToString()));
					return false;
				}
				creator_ = opCreator;
			}


			if (opAppName!=null)
			{
				if (!IsFileNamble(opAppName))
				{
					ShowError(
						string.Format(Properties.Resources._0_MUST_NOT_INCLUDE_1,
						"appname",
						GetFileInamableChars().ToString()));
					return false;
				}
				appName_ = opAppName;
			}
			if (string.IsNullOrEmpty(AppName))
			{
				ShowError(Properties.Resources.APPNAME_MUST_BE_SPECIFIED);
				return false;
			}


	






			if (extra.Count != 0)
			{
				StringBuilder sb = new StringBuilder();
				sb.AppendLine(Properties.Resources.UNKNOWN_OPTION);
				sb.AppendLine(extra[0]);
				ShowError(sb.ToString());
				return false;
			}

			return true;
		}

		public static string Title
		{
			get
			{
				return _title;
			}
		}
		public static string Section
		{
			get { return section_; }
		}
		static string ThisDir
		{
			get
			{
				return System.IO.Path.GetDirectoryName(System.Windows.Forms.Application.ExecutablePath);
			}
		}
		public static string UserIniFullpath
		{
			get
			{
				return System.IO.Path.Combine(ThisDir, userInifileName_);
			}
		}
		public static string Creator
		{
			get { return creator_; }
		}
		public static string AppName
		{
			get { return appName_; }
		}

		public static int DefaultPathType
		{
			get { return defaultpathtype_; }
		}
		public static string DefaultPath0
		{
			get
			{
				if (string.IsNullOrEmpty(defaultpath0_))
				{
					return System.IO.Path.GetDirectoryName(System.Windows.Forms.Application.ExecutablePath);
				}
				if (!System.IO.Path.IsPathRooted(defaultpath0_))
				{
					return System.IO.Path.Combine(
						System.IO.Path.GetDirectoryName(System.Windows.Forms.Application.ExecutablePath),
						defaultpath0_);
				}
				return defaultpath0_;
			}
		}
		public static string DefaultPath3
		{
			get { return defaultpath3_; }
		}
    }
}
