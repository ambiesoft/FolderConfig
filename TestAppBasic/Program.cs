//BSD 2-Clause License
//
//Copyright (c) 2017, Ambiesoft
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//
//* Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//* Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;

using Ambiesoft;
namespace TestAppBasic
{
    static class Program
    {
        static bool MakeUserChoosePath()
        {
            string folderconfigexe = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), "FolderConfig.exe");
            Process pro = Process.Start(folderconfigexe);
            pro.WaitForExit();
            if (!FolderConfigHelper.IsDialogResultOK(pro.ExitCode))
            {
                // user selected cancel.
                return false;
            }
            Debug.Assert(FolderConfigHelper.UserSettingExists);
            return true;
        }

       


        [STAThread]
        static void Main()
        {
            string settingFolder = null;
            bool ok = false;
            try
            {
                do
                {
                    if (!FolderConfigHelper.UserSettingExists)
                    {
                        // this is the first time user launch app
                        if (!MakeUserChoosePath())
                            return;
                    }

                    settingFolder = FolderConfigHelper.GetConfigPath();

                    // check folder accessibility,
                    // FolderConfig has setting of user defined folder, there is a chance
                    // it can not be accessible.
                    // This function actually write a file in the folder (test.ini).
                    if (!FolderConfigHelper.IsFolderAccessible(settingFolder))
                    {
                        if (DialogResult.Yes != MessageBox.Show(
                            string.Format("\"{0}\" could not be accessed. Do you want to configure folder now?", settingFolder),
                            Application.ProductName,
                            MessageBoxButtons.YesNo,
                            MessageBoxIcon.Exclamation))
                        {
                            return;
                        }

                        if (!MakeUserChoosePath())
                            return;

                        continue;
                    }
                    ok = true;
                } while (!ok);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message,
                    Application.ProductName,
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
                return;
            }
            
            Debug.Assert(settingFolder != null);
            Debug.Assert(Directory.Exists(settingFolder));

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            // Application.Run(new Form1(settingFolder));
            new Form1(settingFolder).ShowDialog();
        }
    }
}
