using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace FolderConfigLauncher
{
    static class Program
    {
        [System.Runtime.InteropServices.DllImport("user32.dll")]
        private static extern bool SetProcessDPIAware();

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static int Main(string[] args)
        {
            if(Environment.OSVersion.Version.Major >= 6)
                SetProcessDPIAware();

            return Ambiesoft.FolderConfig.Program.libmain(args);
        }
    }
}
