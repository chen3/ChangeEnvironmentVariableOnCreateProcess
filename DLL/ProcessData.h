#pragma once

#include "Lazy.hpp"

namespace ChangeEnvironmentVariableOnCreateProcess {
    class ProcessData
    {
    public:
        ProcessData() = delete;
        ProcessData& operator=(const ProcessData&) = delete;
        ProcessData(const ProcessData&) = delete;

        static ChangeEnvironmentVariableOnCreateProcess::Lazy<unsigned long> currentProcessID;
        static ChangeEnvironmentVariableOnCreateProcess::Lazy<std::wstring> currentProcessMainMuduleFilePath;
        static ChangeEnvironmentVariableOnCreateProcess::Lazy<std::wstring> currentProcessMainMuduleFileDescription;
        static ChangeEnvironmentVariableOnCreateProcess::Lazy<std::wstring> currentProcessMainMuduleFileVersion;
        static ChangeEnvironmentVariableOnCreateProcess::Lazy<std::wstring> currentProcessMainMuduleProductVersion;
        static ChangeEnvironmentVariableOnCreateProcess::Lazy<std::wstring> currentProcessMainMuduleFileName;
    };
}
