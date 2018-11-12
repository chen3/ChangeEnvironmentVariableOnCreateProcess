#include "stdafx.h"
#include "StringHelper.h"

std::string ChangeEnvironmentVariableOnCreateProcess::wstringToUtf8(const std::wstring & str) noexcept
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}
