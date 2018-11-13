#include "stdafx.h"
#include "FileAttribute.h"
#include "Lazy.hpp"
#include "ProcessData.h"

using std::wstring;
using ChangeEnvironmentVariableOnCreateProcess::ProcessData;
using std::experimental::filesystem::path;
using ChangeEnvironmentVariableOnCreateProcess::Lazy;

Lazy<unsigned long> ChangeEnvironmentVariableOnCreateProcess::ProcessData::currentProcessID = 
    Lazy<unsigned long>([]() -> unsigned long { return ::GetCurrentProcessId(); });
Lazy<wstring> ChangeEnvironmentVariableOnCreateProcess::ProcessData::currentProcessMainMuduleFilePath =
    Lazy<wstring>([]() {
        HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ProcessData::currentProcessID.value());
        wchar_t name[1024];
        unsigned long length = ::GetModuleFileNameEx(hProcess, NULL, name, sizeof(name));
        if (length <= 0) {
            return wstring();
        }
        wstring sname(name, length);
        return sname;
    });
Lazy<wstring> ChangeEnvironmentVariableOnCreateProcess::ProcessData::currentProcessMainMuduleFileDescription =
    Lazy<wstring>([]() {
        wstring result;
        if (ProcessData::currentProcessMainMuduleFilePath.value().empty()) {
            return result;
        }
        BaseFlow::Attribute::GetFileDescription(ProcessData::currentProcessMainMuduleFilePath.value(), result);
        return result;
    });
Lazy<wstring> ChangeEnvironmentVariableOnCreateProcess::ProcessData::currentProcessMainMuduleFileVersion = 
    Lazy<wstring>([]() {
        wstring result;
        if (ProcessData::currentProcessMainMuduleFilePath.value().empty()) {
            return result;
        }
        BaseFlow::Attribute::GetFileVersion(ProcessData::currentProcessMainMuduleFilePath.value(), result);
        return result;
    });
Lazy<wstring> ChangeEnvironmentVariableOnCreateProcess::ProcessData::currentProcessMainMuduleProductVersion =
    Lazy<wstring>([]() {
        wstring result;
        if (ProcessData::currentProcessMainMuduleFilePath.value().empty()) {
            return result;
        }
        BaseFlow::Attribute::GetProductVersion(ProcessData::currentProcessMainMuduleFilePath.value(), result);
        return result;
    });
Lazy<wstring> ChangeEnvironmentVariableOnCreateProcess::ProcessData::currentProcessMainMuduleFileName = 
    Lazy<wstring>([]() {
        path appDataPath = ProcessData::currentProcessMainMuduleFilePath.value();
        return appDataPath.filename().wstring();
    });