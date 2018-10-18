
#include "stdafx.h"

#include "../../lsMisc/HighDPI.h"

using namespace System;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	Ambiesoft::InitHighDPISupport();

	return Ambiesoft::FolderConfig::Program::main(args);
}
