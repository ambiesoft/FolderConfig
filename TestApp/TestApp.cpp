// TestApp.cpp : main project file.

#include "stdafx.h"

using namespace System;
using namespace Ambiesoft;
int main(array<System::String ^> ^args)
{
	String^ path = FolderConfigHelper::GetConfigPath();

	Console::WriteLine(String::Format("path is \"{0}\".",path));
    return 0;
}
