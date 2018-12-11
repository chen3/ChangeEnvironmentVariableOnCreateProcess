#pragma once

#include <filesystem>

namespace mx404 {
    namespace PathHelper {
        std::wstring getApplicationFullPath(const wchar_t* applicationName, const wchar_t* commandLine);
    }
}