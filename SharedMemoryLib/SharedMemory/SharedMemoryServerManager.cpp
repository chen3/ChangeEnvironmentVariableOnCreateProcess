#include "stdafx.h"
#include "SharedMemoryServerManager.h"

#include "EnvChange.h"
#include "IEnvChange.hpp"
#include "MemData.h"
#include "MemDataExtension.h"
#include "SharedMemoryServerManagerException.hpp"
#include "SharedMemoryObject.h"
#include "SharedMemoryObjectCreateException.hpp"
#include "SharedMemoryObjectException.hpp"

using boost::interprocess::windows_shared_memory;
using boost::interprocess::managed_windows_shared_memory;
using boost::interprocess::mapped_region;
using std::string;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryServerManagerException;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryServerManager;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryObject;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryObjectCreateException;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryObjectException;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::IEnvChange;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemData;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemDataExtension;
template<class T>
using BoostAlloc = boost::interprocess::allocator<T, boost::interprocess::managed_windows_shared_memory::segment_manager>;

SharedMemoryServerManager::SharedMemoryServerManager(const std::string& name)
    try : name(name)
    , mainShdMem(std::make_shared<windows_shared_memory>(boost::interprocess::open_or_create, this->name.c_str(), boost::interprocess::read_write, 4))
    , mainRegion(std::make_shared<mapped_region>(*mainShdMem, boost::interprocess::read_write))
    , latestShdMemVersion(static_cast<uint32_t*>(mainRegion->get_address()))
{
}
catch (boost::interprocess::interprocess_exception&) {
    BOOST_THROW_EXCEPTION(SharedMemoryServerManagerException()
        << boost::errinfo_nested_exception(boost::current_exception()));
}

inline void SharedMemoryServerManager::setLatestVersion(uint32_t value)
{
    *latestShdMemVersion = value;
}

inline uint32_t SharedMemoryServerManager::getLatestVersion() const
{
    return *latestShdMemVersion;
}

void SharedMemoryServerManager::updateData(std::shared_ptr<MemData> data)
{
    uint32_t v = getLatestVersion();
    for (uint32_t i = 1; i != 0; ++i) {
        try {
            uint32_t newValue = v + i;
            // std::make_shared 涉及友元
            SharedMemoryObject* temp = new SharedMemoryObject(name + std::to_string(newValue), MemDataExtension::getSugeestMemoryByteSize(data));
            subSharedMem = std::shared_ptr<SharedMemoryObject>(temp);
            subSharedMem->updateData(data);
            setLatestVersion(newValue);
            return;
        }
        catch (SharedMemoryObjectCreateException&) { // 创建失败则忽略并继续，不影响继续执行，几乎无法触发该异常
            if (!!subSharedMem) {
                subSharedMem.reset();
            }
        }
    }
    BOOST_THROW_EXCEPTION(SharedMemoryServerManagerException()
        << SharedMemoryException::ExceptionInfoString("Update data fail, can't open children shared memory."));
}

