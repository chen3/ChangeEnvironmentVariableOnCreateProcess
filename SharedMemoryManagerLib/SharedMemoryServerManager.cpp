#include "stdafx.h"
#include "SharedMemoryServerManager.h"

#include "MemData.h"
#include "SharedMemoryServerManagerException.h"

using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemoryManager::MemData;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemoryManager::SharedMemoryServerManager;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemoryManager::SharedMemoryServerManagerException;
using UnmanagerSharedMemoryServerManager = mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryServerManager;
using msclr::interop::marshal_as;

SharedMemoryServerManager::SharedMemoryServerManager(System::String^ name)
{
    try {
        if (System::String::IsNullOrWhiteSpace(name)) {
            throw gcnew System::ArgumentException("must not be null or empty", "name");
        }
        manager = new UnmanagerSharedMemoryServerManager(marshal_as<std::string>(name));
    }
    catch (std::exception& ex) {
        throw gcnew SharedMemoryServerManagerException(gcnew System::String(ex.what()));
    }
}

SharedMemoryServerManager::~SharedMemoryServerManager() 
{
    delete manager;
}

inline System::UInt32 SharedMemoryServerManager::GetLatestVersion()
{
    return manager->getLatestVersion();
}

void SharedMemoryServerManager::UpdateData(MemData ^ data)
{
    manager->updateData(data->convert());
}
