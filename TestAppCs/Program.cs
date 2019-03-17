using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.IO;

using Ambiesoft;

namespace TestAppCs
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            string inifile = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath),
                "folder.ini");

            StringBuilder sbMessage = new StringBuilder();

            int pathtype;
            Profile.GetInt("Main", "PathType", 0, out pathtype, inifile);
            sbMessage.Append("PathType=");
            sbMessage.Append(pathtype);
            sbMessage.AppendLine();

            if (pathtype == 3)
            {
                string folder;
                Profile.GetString("Main", "folder", string.Empty, out folder, inifile);
                sbMessage.Append("folder=");
                sbMessage.Append(folder);
                sbMessage.AppendLine();
            }
        

            MessageBox.Show(sbMessage.ToString());
        }
    }
}
