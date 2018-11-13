// FileAttribute.cpp: 获取文件-属性-详细信息
// by liwen  
// https://blog.csdn.net/liwen930723/article/details/49471459
//////////////////////////////////////////////////////////////////////  
#include "stdafx.h"
#include "FileAttribute.h"
#pragma comment(lib, "Mincore.lib")

namespace {
    bool QueryValue(const std::wstring& ValueName, const std::wstring& szModuleName, std::wstring& RetStr)
    {
        bool bSuccess = false;
        BYTE* m_lpVersionData = nullptr;
        DWORD m_dwLangCharset = 0;
        wchar_t *tmpstr = nullptr;

        do {
            if (ValueName.empty() || szModuleName.empty()) {
                break;
            }

            DWORD dwHandle;
            // 判断系统能否检索到指定文件的版本信息
            DWORD dwDataSize = ::GetFileVersionInfoSizeW(szModuleName.c_str(), &dwHandle);
            if (dwDataSize == 0) {
                break;
            }

            m_lpVersionData = new (std::nothrow) BYTE[dwDataSize];  // 分配缓冲区
            if (nullptr == m_lpVersionData) {
                break;
            }

            // 检索信息
            if (!::GetFileVersionInfoW(szModuleName.c_str(), dwHandle, dwDataSize, m_lpVersionData)) {
                break;
            }

            unsigned int nQuerySize;
            DWORD* pTransTable;
            // 设置语言
            if (!::VerQueryValueW(m_lpVersionData, L"\\VarFileInfo\\Translation", (void**)(&pTransTable), &nQuerySize)) {
                break;
            }

            m_dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
            if (m_lpVersionData == nullptr) {
                break;
            }

            tmpstr = new (std::nothrow) wchar_t[128];// 分配缓冲区
            if (nullptr == tmpstr) {
                break;
            }
            swprintf_s(tmpstr, 128, L"\\StringFileInfo\\%08lx\\%s", m_dwLangCharset, ValueName.c_str());
            void* lpData;

            // 调用此函数查询前需要先依次调用函数GetFileVersionInfoSize和GetFileVersionInfo
            if (::VerQueryValueW(static_cast<void*>(m_lpVersionData), tmpstr, &lpData, &nQuerySize)) {
                RetStr = static_cast<wchar_t*>(lpData);
            }

            bSuccess = true;
        } while (false);

        // 销毁缓冲区
        if (m_lpVersionData != nullptr) {
            delete[] m_lpVersionData;
            m_lpVersionData = nullptr;
        }
        if (tmpstr != nullptr) {
            delete[] tmpstr;
            tmpstr = nullptr;
        }

        return bSuccess;
    }
}

//获取文件说明
bool BaseFlow::Attribute::GetFileDescription(const std::wstring& szModuleName, std::wstring& RetStr) { 
    return QueryValue(L"FileDescription", szModuleName, RetStr); 
};   
//获取文件版本
bool BaseFlow::Attribute::GetFileVersion(const std::wstring& szModuleName, std::wstring& RetStr) {
    return QueryValue(L"FileVersion", szModuleName, RetStr); 
};	   	
//获取内部名称
bool BaseFlow::Attribute::GetInternalName(const std::wstring& szModuleName, std::wstring& RetStr) { 
    return QueryValue(L"InternalName", szModuleName, RetStr); 
};	   
//获取公司名称
bool BaseFlow::Attribute::GetCompanyName(const std::wstring& szModuleName, std::wstring& RetStr) { 
    return QueryValue(L"CompanyName", szModuleName, RetStr); 
};	   
//获取版权
bool BaseFlow::Attribute::GetLegalCopyright(const std::wstring& szModuleName, std::wstring& RetStr) { 
    return QueryValue(L"LegalCopyright", szModuleName, RetStr); 
};    
//获取原始文件名
bool BaseFlow::Attribute::GetOriginalFilename(const std::wstring& szModuleName, std::wstring& RetStr) { 
    return QueryValue(L"OriginalFilename", szModuleName, RetStr); 
};  
//获取产品名称
bool BaseFlow::Attribute::GetProductName(const std::wstring& szModuleName, std::wstring& RetStr) { 
    return QueryValue(L"ProductName", szModuleName, RetStr); 
};	   
//获取产品版本
bool BaseFlow::Attribute::GetProductVersion(const std::wstring& szModuleName, std::wstring& RetStr) {
    return QueryValue(L"ProductVersion", szModuleName, RetStr); 
};    