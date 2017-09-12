#pragma once


#pragma warning(disable: 4100)

#include <cassert>
#include <string>


#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


#include "../lsMisc/DebugMacro.h"
#include "../lsMisc/IsFilenamable.h"
#include "../lsMisc/cppclr/clrString.h"

#include "debugflags.h"

#include "ResUtil.h"
#define I18N(t) ResUtil::getString(t)


