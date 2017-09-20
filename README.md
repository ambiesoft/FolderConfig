# FolderConfig
FolderConfig can make a user of the application choose a folder they want to save app's settings. Application creator does not need to think about where settings are to be saved.

# Download
Download binaries from https://github.com/erasoni/FolderConfig/releases.

# How to deploy
* Extract *FolderConfig-x.x.exe* to your application folder. Place *FolderConfig.exe* in the same directory of your application's main executable resides.
* Rename *FolderConfig.ini.sample* to *FolderConfig.ini* and edit it. 

## FolderConfig.ini
This file holds default values. Application creator should completes this file in accordance with its traits. It should not be editted during runtime of the application. *FolderConfig.ini* looks like follows.

```ini
[Main]
appname=MyApp
creator=Ambiesoft
defaultpathtype=0
defaultpath0=conf
```

If you lanuch *FolderConfig.exe* with *FolderConfig.ini* above, you will see follwing the setting dialog.


