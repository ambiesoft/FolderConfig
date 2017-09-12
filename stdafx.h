#pragma once


#pragma warning(disable: 4100)

#include <cassert>
#include <string>


#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <comdef.h> 
#include <tchar.h>
#include <objbase.h>
#include <shlobj.h>
#include <ShObjIdl.h>
#include <vcclr.h>
#include <string>
#include <io.h>
#include <windows.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <string>

#ifdef _DEBUG
#include <cassert>
#endif

#include <stlsoft/smartptr/scoped_handle.hpp>


#include "../lsMisc/DebugMacro.h"
#include "../lsMisc/IsFilenamable.h"
#include "../lsMisc/cppclr/clrString.h"

#include "debugflags.h"

#include "ResUtil.h"
#define I18N(t) ResUtil::getString(t)


