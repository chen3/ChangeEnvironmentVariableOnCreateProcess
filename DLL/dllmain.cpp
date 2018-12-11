// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "PathHelper.h"
#include "ProcessData.h"
#include "ProcessEnvironment.h"

namespace {
    using ChangeEnvironmentVariableOnCreateProcess::ProcessData;
    using mx404::ChangeEnvironmentVariableOnCreateProcess::IProcessEnvironment;
    using mx404::ChangeEnvironmentVariableOnCreateProcess::ProcessEnvironment;
    using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::IEnvChange;
    using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryClientManager;
    namespace PathHelper = mx404::PathHelper;

    SharedMemoryClientManager clientManager("MX404ChangeEnvironmentVariableOnCreateProcess");

    decltype(&::CreateProcessW) fpCreateProcess = nullptr;
    BOOL detourCreateProcess(
        _In_opt_     LPCWSTR lpApplicationName,
        _Inout_opt_  LPWSTR lpCommandLine,
        _In_opt_     LPSECURITY_ATTRIBUTES lpProcessAttributes,
        _In_opt_     LPSECURITY_ATTRIBUTES lpThreadAttributes,
        _In_         BOOL bInheritHandles,
        _In_         DWORD dwCreationFlags,
        _In_opt_     LPVOID lpEnvironment,
        _In_opt_     LPCWSTR lpCurrentDirectory,
        _In_         LPSTARTUPINFOW lpStartupInfo,
        _Out_        LPPROCESS_INFORMATION lpProcessInformation) {
        std::wstring envStr;
        do {
            try {
                if (!clientManager.contain()) {
                    break;
                }
                std::wstring targetFileFullPath = PathHelper::getApplicationFullPath(lpApplicationName, lpCommandLine);
                if (targetFileFullPath.empty()) {
                    break;
                }
                std::shared_ptr<IEnvChange> envChange = clientManager.getEnvChange(targetFileFullPath);
                if (envChange == nullptr) {
                    break;
                }

                std::shared_ptr<ProcessEnvironment> pEnv;
                if (lpEnvironment != nullptr) {
                    std::shared_ptr<wchar_t> envPtr(static_cast<wchar_t*>(lpEnvironment), [](wchar_t*) {});
                    pEnv = std::make_shared<ProcessEnvironment>(ProcessEnvironment::getFormProcessString(envPtr));
                }
                else {
                    pEnv = std::make_shared<ProcessEnvironment>(ProcessEnvironment::getFormCurrentProcess());
                }

                std::shared_ptr<IProcessEnvironment> newPEnv = envChange->change(pEnv);
                envStr = newPEnv->toWinAPINeedString();
            }
            catch (...) {
                envStr.clear();
            }
        } while (false);
        return fpCreateProcess(lpApplicationName, lpCommandLine, lpProcessAttributes,
                lpThreadAttributes, bInheritHandles, 
                envStr.empty() ? dwCreationFlags : dwCreationFlags | CREATE_UNICODE_ENVIRONMENT,
                envStr.empty() ? lpEnvironment : static_cast<void*>(const_cast<wchar_t*>(envStr.c_str())),
                lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    };

    bool initializeAndEnableHook() {
        if (::MH_Initialize() != ::MH_STATUS::MH_OK) {
            return false;
        }
        if (::MH_CreateHook(&CreateProcessW, &detourCreateProcess,
            reinterpret_cast<LPVOID*>(&fpCreateProcess)) != MH_STATUS::MH_OK) {
            return false;
        }
        if (::MH_EnableHook(&CreateProcessW) != ::MH_STATUS::MH_OK) {
            return false;
        }
        return true;
    }

    bool disableHookAndUninitialize() {
        ::MH_STATUS disableResult = ::MH_DisableHook(&CreateProcessW);
        ::MH_STATUS uninitializeResult = ::MH_Uninitialize();
        return disableResult == ::MH_STATUS::MH_OK && uninitializeResult == ::MH_STATUS::MH_OK;
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    UNREFERENCED_PARAMETER(lpReserved);
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH: {
            ::DisableThreadLibraryCalls(hModule);
            initializeAndEnableHook();
        } break;
        case DLL_PROCESS_DETACH: {
            disableHookAndUninitialize();
        } break;
    }
    return TRUE;
}

