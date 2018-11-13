// FileAttribute.h: interface for the FileAttribute 
// by xiboliya
// https://blog.csdn.net/liwen930723/article/details/49471459
//////////////////////////////////////////////////////////////////////  
#pragma once
#include <string>

namespace BaseFlow
{
    namespace Attribute
    {
        bool	GetFileDescription(const std::wstring& szModuleName, std::wstring& RetStr);
        bool	GetFileVersion(const std::wstring& szModuleName, std::wstring& RetStr);
        bool	GetInternalName(const std::wstring& szModuleName, std::wstring& RetStr);
        bool	GetCompanyName(const std::wstring& szModuleName, std::wstring& RetStr);
        bool	GetLegalCopyright(const std::wstring& szModuleName, std::wstring& RetStr);
        bool	GetOriginalFilename(const std::wstring& szModuleName, std::wstring& RetStr);
        bool	GetProductName(const std::wstring& szModuleName, std::wstring& RetStr);
        bool	GetProductVersion(const std::wstring& szModuleName, std::wstring& RetStr);
    }
}