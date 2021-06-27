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
        private static string title_;
        private static string creator_;
        private static string appName_;
        static string appSection_;
        static string userSection_;
        private static int defaultpathtype_;
        private static string defaultpath0_;
        private static string defaultpath3_;
        
        public static string DebugOperation = null;
        public static bool NoOkMessage = false;

        static string getHelpMessage()
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine(Properties.Resources.USAGE + ":");
            sb.AppendLine("FolderConfig.exe [/section Section] [/title Title] /defaultpathtype Defaultpathtype [/defaultpath0 Defaultpath0] [/defaultpath3 Defaultpath3] [/creator Creator] [/appname Appname] [/culture Culture] [/nookmessage]");
            sb.AppendLine();

            sb.AppendLine(Properties.Resources.HELP_SECTION_NAME_OF_CONFIG);

            sb.AppendLine(Properties.Resources.HELP_TITLE);
            // sb.AppendLine(Properties.Resources.HELP_INIFILE);

            sb.AppendLine(Properties.Resources.HELP_DEFAULT_FILE);
            sb.AppendLine(Properties.Resources.HELP_DEFAULT_PATH0);
            sb.AppendLine(Properties.Resources.HELP_DEFAULT_PATH3);
            sb.AppendLine(Properties.Resources.HELP_CREATOR);
            sb.AppendLine(Properties.Resources.HELP_APPNAME);
            sb.AppendLine(Properties.Resources.HELP_CULTURE);
            sb.AppendLine(Properties.Resources.HELP_NOOKMESSAGE);

            return sb.ToString();
        }

        static void ShowError(string s)
        {
            MessageBox.Show(s + "\r\n\r\n" + getHelpMessage(),
                Program.ProductName,
                MessageBoxButtons.OK,
                MessageBoxIcon.Error);
        }
        static void Alert(string msg)
        {
            MessageBox.Show(msg,
                Program.ProductName,
                MessageBoxButtons.OK,
                MessageBoxIcon.Warning);
        }
        static void Alert(Exception ex)
        {
            Alert(ex.Message);
        }

        static string GetRelativePathInamableCharsString()
        { return ":*?\"<>|"; }
        static bool IsRelativePathNamble(string name)
        {
            if (string.IsNullOrEmpty(name))
                return false;

            return name.IndexOfAny(GetRelativePathInamableCharsString().ToCharArray()) < 0;
        }

        static string GetFileInamableCharsString()
        {
            return "/\\:*?\"<>|";
        }
        static bool IsFileNamble(string name)
        {
            if (string.IsNullOrEmpty(name))
                return false;

            return name.IndexOfAny(GetFileInamableCharsString().ToCharArray()) < 0;
        }

        static void SetCulture(string culture)
        {
            if (string.IsNullOrEmpty(culture))
                return;

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
        public static bool init(string[] args)
        {
            string culture = null;

            // Get section from commandline to determin AppSection
            {
                var optionSet = new OptionSet() {
                    {
                        "section=",
                        v => {
                            appSection_=v;
                        }
                    },
                };
                optionSet.Parse(args);
            }

            // First, Read from 'FolderConfig.ini'
            {
                string inipath = Path.Combine(
                    Path.GetDirectoryName(Application.ExecutablePath),
                    Program.ProductName + ".ini");

                HashIni ini = Profile.ReadAll(inipath);

                Profile.GetString(AppSection, "title", null, out title_, ini);
                Profile.GetString(AppSection, "appname", null, out appName_, ini);
                Profile.GetString(AppSection, "creator", null, out creator_, ini);
                Profile.GetInt(AppSection, "defaultpathtype", -1, out defaultpathtype_, ini);
                Profile.GetString(AppSection, "defaultpath0", null, out defaultpath0_, ini);
                Profile.GetString(AppSection, "defaultpath3", null, out defaultpath3_, ini);
                Profile.GetString(AppSection, "culture", null, out culture, ini);
                Profile.GetString(AppSection, "section", userSection_, out userSection_, ini);
#if DEBUG
                if (!String.IsNullOrEmpty(title_))
                    System.Diagnostics.Debug.Print("title is \"{0}\" (from \"{1}\")", title_, inipath);
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
                if (!String.IsNullOrEmpty(userSection_))
                    System.Diagnostics.Debug.Print("section is \"{0}\" (from \"{1}\")", userSection_, inipath);
#endif
            }

            // Early for culture for error message
            SetCulture(culture);

            // Next Read from command line
            string opTitle = null;
            string opDefaultPathType = null;
            string opDefaultPath0 = null;
            string opDefaultPath3 = null;
            string opCreator = null;
            string opAppName = null;
            string opUserSection = null;
            string opCulture = null;
            bool opHelp = false;
            string opDebug = null;
            bool opNoOkMessage = false;

            string errorOp = null;
            var p = new OptionSet() {
                    {
                        "title=",
                        v => {
                            if(!string.IsNullOrEmpty(opTitle))
                            {
                                errorOp="title";
                            }
                            opTitle = v;
                        }
                    },
                    {
                        "defaultpathtype=",
                        v => {
                            if(!string.IsNullOrEmpty(opDefaultPathType))
                            {
                                errorOp="defaultpathtype";
                            }
                            opDefaultPathType=v;
                        }
                    },
                    {
                        "defaultpath0=",
                        v => {
                            if(!string.IsNullOrEmpty(opDefaultPath0))
                            {
                                errorOp="defaultpath0";
                            }
                            opDefaultPath0=v;
                        }
                    },
                    {
                        "defaultpath3=",
                        v => {
                            if(!string.IsNullOrEmpty(opDefaultPath3))
                            {
                                errorOp="defaultpath3";
                            }
                            opDefaultPath3=v;
                        }
                    },
                    {
                        "creator=",
                        v => {
                            if(!string.IsNullOrEmpty(opCreator))
                            {
                                errorOp="creator";
                            }
                            opCreator=v;
                        }
                    },
                    {
                        "appname=",
                        v => {
                            if(!string.IsNullOrEmpty(opAppName))
                            {
                                errorOp="appname";
                            }
                            opAppName=v;
                        }
                    },
                    {
                        "section=",
                        v => {
                            if(!string.IsNullOrEmpty(opUserSection))
                            {
                                errorOp="section";
                            }
                            opUserSection=v;
                        }
                    },
                    {
                        "culture=",
                        v => {
                            if(!string.IsNullOrEmpty(opCulture))
                            {
                                errorOp="culture";
                            }
                            opCulture=v;
                        }
                    },
                    {
                        "h|?|help|v|version",
                        v => {
                            opHelp=true;
                        }
                    },
                    {
                        "debug=",
                        v => {
                            if(!string.IsNullOrEmpty(opDebug))
                            {
                                errorOp="debug";
                            }
                            opDebug=v;
                        }
                    },
                    {
                        "nookmessage",
                        v => {
                            opNoOkMessage=true;
                        }
                    },

            };

            List<string> extra = p.Parse(args);

            if (errorOp != null)
            {
                ShowError(string.Format(Properties.Resources._0_IS_SET_TWICE, errorOp));
                return false;
            }

            // First, set culture for I18N
            if (opCulture != null)
            {
                culture = opCulture;
            }
            SetCulture(culture);

            if (opHelp)
            {
                System.Reflection.Assembly assembly = System.Reflection.Assembly.GetExecutingAssembly();
                System.Diagnostics.FileVersionInfo fvi = System.Diagnostics.FileVersionInfo.GetVersionInfo(assembly.Location);
                string version = fvi.FileVersion;

                MessageBox.Show(getHelpMessage(),
                    string.Format("{0} v{1}", Program.ProductName, version),
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Information
                    );
                return false;
            }

            if (opTitle != null)
                title_ = opTitle;

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
                        GetRelativePathInamableCharsString()));
                    return false;
                }
                defaultpath0_ = opDefaultPath0;
            }

            if (opDefaultPath3 != null)
                defaultpath3_ = opDefaultPath3;

            if (opCreator != null)
            {
                if (!IsFileNamble(opCreator))
                {
                    ShowError(
                        string.Format(Properties.Resources._0_MUST_NOT_INCLUDE_1,
                        "creator",
                        GetFileInamableCharsString()));
                    return false;
                }
                creator_ = opCreator;
            }


            if (opAppName != null)
            {
                if (!IsFileNamble(opAppName))
                {
                    ShowError(
                        string.Format(Properties.Resources._0_MUST_NOT_INCLUDE_1,
                        "appname",
                        GetFileInamableCharsString()));
                    return false;
                }
                appName_ = opAppName;
            }
            if (string.IsNullOrEmpty(AppName))
            {
                ShowError(Properties.Resources.APPNAME_MUST_BE_SPECIFIED);
                return false;
            }
            if (opUserSection != null)
            {
                userSection_ = opUserSection;
            }

            Settings.NoOkMessage = opNoOkMessage;
            Settings.DebugOperation = opDebug;

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
                return title_;
            }
        }
        public static string AppSection
        {
            get { return appSection_?? "Main"; }
        }
        public static string UserSection
        {
            get { return userSection_?? "Main"; }
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
                return System.IO.Path.Combine(ThisDir, Program.InifileName);
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
