// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#include <codecvt>
#include <filesystem>
#include <functional>
#include <MinHook.h>
#include <nlohmann/json.hpp>
#include <psapi.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <IProcessEnvironment.hpp>
#include <SharedMemory\IEnvChange.hpp>
#include <SharedMemory\SharedMemoryClientManager.h>