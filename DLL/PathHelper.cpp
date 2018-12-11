#include "stdafx.h"
#include "PathHelper.h"

#include "ProcessData.h"

using std::filesystem::path;
namespace filesystem = std::filesystem;

namespace {
    const std::wstring emptyWString;

    class ClearDotAndDotDotException : public std::exception {
    public:
        using std::exception::exception;
    };

    std::vector<std::wstring> split(const std::wstring &s, const wchar_t& delim) {
        std::wstringstream ss(s);
        std::wstring item;
        std::vector<std::wstring> elems;
        while (std::getline(ss, item, delim)) {
            elems.push_back(std::move(item));
        }
        return elems;
    }

    std::wstring searchFilePath(const std::wstring& fileName) {
        // 1. Search the directory from which the application loaded.
        {
            std::wstring currentPath = ChangeEnvironmentVariableOnCreateProcess::ProcessData::currentProcessMainMuduleFilePath.value();
            if (!currentPath.empty()) {
                path p = currentPath;
                p = p.parent_path() / fileName;
                if (filesystem::exists(p)) {
                    return p.wstring();
                }
            }
        }
        // 2. Search the current directory for current process
        {
            path p = filesystem::current_path() / fileName;
            if (filesystem::exists(p)) {
                return p.wstring();
            }
        }
        // 3. Search 32-bit windows system directory
        {
            const int infoBufferSize = 32767;
            wchar_t bufffer[infoBufferSize];
            if (::GetSystemDirectory(bufffer, infoBufferSize)) {
                path p = std::wstring(bufffer);
                p /= fileName;
                if (filesystem::exists(p)) {
                    return p.wstring();
                }
            }
        }
        // 5. Search windows directory
        {
            const int infoBufferSize = 32767;
            wchar_t bufffer[infoBufferSize];
            if (::GetWindowsDirectory(bufffer, infoBufferSize)) {
                path p = std::wstring(bufffer);
                p /= fileName;
                if (filesystem::exists(p)) {
                    return p.wstring();
                }
            }
        }
        // 6. Search PATH environment variable
        {
            wchar_t* temp;
            size_t len;
            if (!::_wdupenv_s(&temp, &len, L"PATH")) {
                std::wstring pathEnv = std::wstring(temp, len);
                delete temp;
                if (!pathEnv.empty()) {
                    for (const std::wstring& s : split(pathEnv, L';')) {
                        if (s.empty()) {
                            continue;
                        }
                        path p = s;
                        p /= fileName;
                        if (filesystem::exists(p)) {
                            return p.wstring();
                        }
                    }
                }
            }
        }
        return emptyWString;
    }

    // 只在windows系统上类似C:\xxx的路径上进行过测试
    std::filesystem::path clearDotAndDotDot(const path& p) {
        assert(!p.empty());
        const std::wstring dotdot = L"..";
        const std::wstring dot = L".";

        std::stack<std::wstring> stack;
        path tempPath = p;
        do {
            path parentPath = tempPath.parent_path();
            if (parentPath.empty() || parentPath == tempPath) {
                break;
            }
            std::wstring fileName = tempPath.filename().wstring();
            if (fileName != dot) {
                stack.push(fileName);
            }
            tempPath = parentPath;
        } while (true);
        while (!stack.empty()) {
            std::wstring fileName = stack.top();
            stack.pop();
            if (fileName == dotdot) {
                if (tempPath.parent_path() == tempPath) {
                    throw ClearDotAndDotDotException(("error when clear " + p.string()).c_str());
                }
                tempPath = tempPath.parent_path();
            }
            else {
                tempPath /= fileName;
            }
        }
        return tempPath;
    }

    std::wstring getApplicationFullPathFromLpApplicationName(const wchar_t* applicationName) {
        assert(applicationName != nullptr);
        std::wstring name(applicationName);
        if (name.empty()) {
            return name;
        }
        if (name[0] == L'\"') {
            if (name[name.length() - 1] != L'\"' || name.length() <= 2) {
                return emptyWString;
            }
            name = name.substr(1, name.length() - 2);
            if (name.find(L'\"') != -1) {
                return emptyWString;
            }
        }
        path appPath = name;
        if (!appPath.is_absolute()) {
            appPath = std::filesystem::current_path() / appPath;
        }
        try {
            return clearDotAndDotDot(appPath).wstring();
        }
        catch (ClearDotAndDotDotException&) {
            return emptyWString;
        }
    }

    std::wstring getApplicationFullPathFromLpCommandLine(const wchar_t* commandLine) {
        assert(commandLine != nullptr);
        std::wstring commandLineStr(commandLine);
        if (commandLineStr.empty()) {
            return emptyWString;
        }

        std::wstring appPathStr;
        if (commandLineStr[0] == L'\"') {
            std::size_t found = commandLineStr.find(L'\"', 1);
            if (found == std::wstring::npos || found == 1) {
                return emptyWString;
            }
            appPathStr = commandLineStr.substr(1, found - 1);
        }
        else {
            std::size_t found = commandLineStr.find(L' ');
            if (found == std::wstring::npos || found == 0) {
                return emptyWString;
            }
            appPathStr = commandLineStr.substr(0, found);
        }
        path appPath = appPathStr;
        if (!appPath.has_extension()) {
            appPath += L".exe";
        }
        if (!appPath.is_absolute()) {
            if (!appPath.parent_path().empty()) {
                appPath = std::filesystem::current_path() / appPath;
            }
            else {
                std::wstring fullPath = searchFilePath(appPath.wstring());
                if (fullPath.empty()) {
                    return emptyWString;
                }
            }
        }
        try {
            return clearDotAndDotDot(appPath).wstring();
        }
        catch (ClearDotAndDotDotException&) {
            return emptyWString;
        }
    }

    
}

std::wstring mx404::PathHelper::getApplicationFullPath(const wchar_t * applicationName, const wchar_t * commandLine)
{
    if (applicationName != nullptr) {
        return getApplicationFullPathFromLpApplicationName(applicationName);
    }
    if (commandLine == nullptr) {
        return emptyWString;
    }
    return getApplicationFullPathFromLpCommandLine(commandLine);
}
