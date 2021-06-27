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
using System.IO;
using System.Windows.Forms;

namespace Ambiesoft
{
    public static class FolderConfigHelper  
    {
        static HashIni hashFolderConfig_;
        static HashIni HashFolderConfig
        {
            get
            {
                if(hashFolderConfig_ != null)
                    return hashFolderConfig_;

                string appdir = Path.GetDirectoryName(Application.ExecutablePath);
                string defini = Path.Combine(appdir, "FolderConfig.ini");
                if(!File.Exists(defini))
                    return null;

                return hashFolderConfig_ = Profile.ReadAll(defini);
            }
        }
        static string GetCreator(string section)
        {
            if(HashFolderConfig==null)
                return null;

            string c;
            Profile.GetString(section,"creator",null,out c,HashFolderConfig);
            return c;
        }
        static string GetApp(string section)
        {
            if(HashFolderConfig==null)
                return null;

            string c;
            Profile.GetString(section,"appname",null,out c,HashFolderConfig);
            return c;
        }

        static string GetSystemPathCommon(string section, Environment.SpecialFolder sf,string creator,string appname)
        {
            if (creator == null)
                creator = GetCreator(section);
            if (appname == null)
                appname = GetApp(section);
            
            if(string.IsNullOrEmpty(creator) || string.IsNullOrEmpty(appname))
                return null;

            string path = Environment.GetFolderPath(sf);
            path = Path.Combine(path, creator);
            path = Path.Combine(path, appname);
            return path;
        }
        static string GetSystemLocalPath(string section,string creator,string appname)
        {
            return GetSystemPathCommon(section, Environment.SpecialFolder.LocalApplicationData,
                creator, appname);
        }
        static string GetSystemAppPath(string section, string creator, string appname)
        {
            return GetSystemPathCommon(section, Environment.SpecialFolder.ApplicationData,
                creator, appname);
        }
        static string GetDefaultConfigPath(string section, int pathtype, string creator, string appname)
        {
            if (pathtype < 0)
            {
                if (HashFolderConfig == null)
                    return null;

                Profile.GetInt(section, "defaultpathtype", -1, out pathtype, HashFolderConfig);
            }

            if(pathtype==0)
            {
                if (HashFolderConfig == null)
                    return null;

                string path;
                Profile.GetString(section, "defaultpath0", "", out path, HashFolderConfig);
                return Path.Combine(AppDir,path);
            }
            else if(pathtype==1)
            {
                return GetSystemLocalPath(section,creator,appname);
            }
            else if(pathtype==2)
            {
                return GetSystemAppPath(section, creator, appname);
            }
            else if(pathtype==3)
            {
                if (HashFolderConfig == null)
                    return null;

                string path;
                Profile.GetString(section, "defaultpath3", null, out path, HashFolderConfig);
                return path;
            }
            return null;
        }
        static string GetDefaultConfigPath(string section)
        {
            return GetDefaultConfigPath(section, -1, null, null);
        }
        public static string GetConfigPath(string section, string creator, string appname)
        {
            string ret = GetConfigPathImpl(section, creator, appname);
            if (string.IsNullOrEmpty(ret))
                return ret;
            EnsureFolder(ret);
            return ret;
        }
        public static string GetConfigPath(string section)
        {
            return GetConfigPath(section, null, null);
        }

        static string AppDir
        {
            get
            {
                return Path.GetDirectoryName(Application.ExecutablePath);
            }
        }

        public static bool GetUserSettingExists(string section)
        {
            string ini = Path.Combine(AppDir, FolderConfig.Program.InifileName);
            if (!File.Exists(ini))
                return false;

            HashIni hashini = Profile.ReadAll(ini);

            int pathtype = -1;
            Profile.GetInt(section, "PathType", -1, out pathtype, hashini);
            if (pathtype < 0)
                return false;
            return true;
        }
        public static bool UserSettingExists
        {
            get
            {
                return GetUserSettingExists("Main");
            }
        }
        public static bool IsDialogResultOK(int retval)
        {
            return (retval & 1) != 0;
        }
        static string GetConfigPathImpl(string section, string creator, string appname)
        {
            string ini = Path.Combine(AppDir, FolderConfig.Program.InifileName);
            if (!File.Exists(ini))
                return GetDefaultConfigPath(section, -1, creator, appname);

            HashIni hashini = Profile.ReadAll(ini);

            int pathtype = -1;
            Profile.GetInt(section, "PathType", -1, out pathtype, hashini);
            if (pathtype < 0)
                return GetDefaultConfigPath(section, -1, creator, appname);

            switch (pathtype)
            {
                case 0:
                case 1:
                case 2:
                    return GetDefaultConfigPath(section, pathtype, creator, appname);

                //case 1:
                //    return GetSystemPathCommon(section, Environment.SpecialFolder.LocalApplicationData);
                //case 2:
                //    return GetSystemPathCommon(section, Environment.SpecialFolder.ApplicationData);
                case 3:
                    string path = null;
                    Profile.GetString(section, "folder", null, out path, hashini);
                    if (!string.IsNullOrEmpty(path))
                        return path;
                    Profile.GetString(section, "defaultpath3", null, out path, HashFolderConfig);
                    return path;
            }
            return null;
        }
        public static string GetConfigPath()
        {
            return GetConfigPath("Main");
        }

        static bool IsFullPath(string folder)
        {
            string a = Path.Combine(@"C:\TestPath\", folder);
            return a == folder;
        }

        static void EnsureFolder(string folder)
        {
            if (Directory.Exists(folder))
                return;
            try
            {
                Directory.CreateDirectory(folder);
            }
            catch (Exception)
            {
            }
        }

        /// <summary>
        /// Check accessibility by actually writing to a file in the folder.
        /// folder: must be a full path
        /// </summary>
        /// <param name="folder"></param>
        /// <returns></returns>
        static public bool IsFolderAccessible(string folder)
        {
            if (!IsFullPath(folder))
                throw new Exception("Folder must be full path");

            EnsureFolder(folder);
            string testini = Path.Combine(folder, "test.ini");
            string data = Guid.NewGuid().ToString();
            Profile.WriteString("test", "data", data, testini);

            string getdata;
            Profile.GetString("test", "data", null, out getdata, testini);
            return getdata == data;
        }
    }
}
