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

        static string GetSystemPathCommon(string section, Environment.SpecialFolder sf)
        {
            string creator=GetCreator(section);
            string appname=GetApp(section);
            if(string.IsNullOrEmpty(creator) || string.IsNullOrEmpty(appname))
                return null;

            string path = Environment.GetFolderPath(sf);
            path = Path.Combine(path, creator);
            path = Path.Combine(path, appname);
            return path;
        }
        static string GetSystemLocalPath(string section)
        {
            return GetSystemPathCommon(section, Environment.SpecialFolder.LocalApplicationData);
        }
        static string GetSystemAppPath(string section)
        {
            return GetSystemPathCommon(section, Environment.SpecialFolder.ApplicationData);
        }
        static string GetDefaultConfigPath(string section)
        {
            string appdir = Path.GetDirectoryName(Application.ExecutablePath);
            //string defini = Path.Combine(appdir, "FolderConfig.ini");
            //if(!File.Exists(defini))
            //    return null;

            if (HashFolderConfig == null)
                return null;

            int pathtype;
            Profile.GetInt(section, "defaultpathtype", 0, out pathtype, HashFolderConfig);
            if(pathtype==0)
            {
                string path;
                Profile.GetString(section, "defaultpath0", "", out path, HashFolderConfig);
                return Path.Combine(appdir,path);
            }
            else if(pathtype==1)
            {
                return GetSystemLocalPath(section);
            }
            else if(pathtype==2)
            {
                return GetSystemAppPath(section);
            }
            else if(pathtype==3)
            {
                string path;
                Profile.GetString(section, "defaultpath3", null, out path, HashFolderConfig);
                return path;
            }
            return null;
        }

        public static string GetConfigPath(string section)
        {
            string appdir = Path.GetDirectoryName(Application.ExecutablePath);
            string ini = Path.Combine(appdir, "folder.ini");
            if (!File.Exists(ini))
                return GetDefaultConfigPath(section);

            HashIni hashini = Profile.ReadAll(ini);

            int pathtype = 0;
            Profile.GetInt(section, "PathType", 0, out pathtype, hashini);
            switch (pathtype)
            {
                case 0:
                    string defpath = null;
                    Profile.GetString(section, "defaultpath0", null, out defpath, HashFolderConfig);
                    return Path.Combine(appdir, defpath);

                case 1:
                    return GetSystemPathCommon(section, Environment.SpecialFolder.LocalApplicationData);
                case 2:
                    return GetSystemPathCommon(section, Environment.SpecialFolder.ApplicationData);
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

        /// <summary>
        /// Check accessibility by actually writing to a file in the folder.
        /// folder: must be a full path
        /// </summary>
        /// <param name="folder"></param>
        /// <returns></returns>
        static public bool IsFolderAccessable(string folder)
        {
            if (!IsFullPath(folder))
                throw new Exception("Folder must be full path");

            string testini = Path.Combine(folder, "test.ini");
            string data = Guid.NewGuid().ToString();
            Profile.WriteString("test", "data", data, testini);

            string getdata;
            Profile.GetString("test", "data", null, out getdata, testini);
            return getdata == data;
        }
    }
}
