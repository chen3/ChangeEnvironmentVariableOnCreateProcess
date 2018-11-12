#pragma once

namespace ChangeEnvironmentVariableOnCreateProcess {

    // https://stackoverflow.com/questions/4358870/convert-wstring-to-string-encoded-in-utf-8
    // convert wstring to UTF-8 string
    std::string wstringToUtf8(const std::wstring& str) noexcept;

}