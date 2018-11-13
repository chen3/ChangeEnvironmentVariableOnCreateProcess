#include "stdafx.h"
#include "ProcessEnvironment.h"
#include "StringHelper.h"

using ChangeEnvironmentVariableOnCreateProcess::ProcessEnvironment;
using std::shared_ptr;
using std::wstringstream;
using std::move;
using std::vector;
using std::wstring;
using std::unordered_map;
using ChangeEnvironmentVariableOnCreateProcess::wstringToUtf8;

ProcessEnvironment::ProcessEnvironment(const ProcessEnvironment && env) noexcept
    : env(move(env.env))
{
}

ProcessEnvironment ProcessEnvironment::getFormCurrentProcess()
{
    shared_ptr<wchar_t> envPtr(::GetEnvironmentStringsW(), [](wchar_t* e) { ::FreeEnvironmentStringsW(e); });
    return getFormProcessString(envPtr);
}

ProcessEnvironment ProcessEnvironment::getFormProcessString(const shared_ptr<wchar_t> env)
{
    ProcessEnvironment pe = ProcessEnvironment();
    for (wchar_t* p = env.get(); *p != L'\0'; ++p) {
        wstringstream s;
        for (; *p != L'\0'; ++p) {
            s << *p;
        }
        vector<wstring> v = splitString(s.str(), L"=");
        if (v.size() != 2) {
            continue;
        }
        pe.env[v[0]] = wstring(v[1]);
    }
    return pe;
}

wstring ProcessEnvironment::toWinAPINeedString() const noexcept
{
    wstringstream s;
    for (auto pair : env) {
        s << pair.first << '=' << pair.second << '\0';
    }
    s << '\0';
    return s.str();
}

wstring& ProcessEnvironment::operator[](const wstring & s) noexcept
{
    unordered_map<wstring, wstring>::iterator i = env.find(s);
    if (i == env.end()) {
        env[s] = L"";
        return env[s];
    }
    return i->second;
}

bool ProcessEnvironment::contains(const wstring & key) const noexcept
{
    return env.find(key) != env.end();
}

nlohmann::json ChangeEnvironmentVariableOnCreateProcess::ProcessEnvironment::toJson() const noexcept
{
    nlohmann::json json;
    for (auto pair : env) {
        json[wstringToUtf8(pair.first)] = wstringToUtf8(pair.second);
    }
    return json;
}

ProcessEnvironment::ProcessEnvironment() noexcept
{
}

vector<wstring> ProcessEnvironment::splitString(const wstring& s, const wstring& c) noexcept
{
    vector<std::wstring> v;
    wstring::size_type pos2 = s.find(c);
    wstring::size_type pos1 = 0;
    while (wstring::npos != pos2) {
        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length()) {
        v.push_back(s.substr(pos1));
    }
    return v;
}
