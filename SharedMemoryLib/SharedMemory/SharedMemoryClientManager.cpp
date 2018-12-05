#include "stdafx.h"
#include "SharedMemoryClientManager.h"

#include "EnvChange.h"
#include "IEnvChange.hpp"
#include "MemData.h"
#include "SharedMemoryClientManagerException.hpp"
#include "SharedMemoryObject.h"
#include "SharedMemoryObjectCreateException.hpp"
#include "SharedMemoryObjectException.hpp"

using boost::interprocess::windows_shared_memory;
using boost::interprocess::managed_windows_shared_memory;
using boost::interprocess::mapped_region;
using std::string;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryClientManagerException;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryClientManager;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryObject;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryObjectCreateException;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryObjectException;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::IEnvChange;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemData;
template<class T>
using BoostAlloc = boost::interprocess::allocator<T, boost::interprocess::managed_windows_shared_memory::segment_manager>;

SharedMemoryClientManager::SharedMemoryClientManager(const std::string& name)
    try : name(name)
    , mainShdMem(std::make_shared<windows_shared_memory>(boost::interprocess::open_or_create, this->name.c_str(), boost::interprocess::read_write, 4))
    , mainRegion(std::make_shared<mapped_region>(*mainShdMem, boost::interprocess::read_only))
    , latestShdMemVersion(static_cast<uint32_t*>(mainRegion->get_address()))
{
}
catch (boost::interprocess::interprocess_exception&) {
    BOOST_THROW_EXCEPTION(SharedMemoryClientManagerException()
        << boost::errinfo_nested_exception(boost::current_exception()));
}

bool SharedMemoryClientManager::contain()
{
    return getLatestVersion() != 0;
}

std::shared_ptr<SharedMemoryObject> SharedMemoryClientManager::getSubSharedMem()
{
    bool firstIsFail = false;
    do {
        uint32_t version = getLatestVersion();
        if (version == 0) {
            break;
        }

        string subName = name + std::to_string(version);
        try {
            std::shared_ptr<SharedMemoryObject> subShdMem = std::make_shared<SharedMemoryObject>(subName);
            return subShdMem;
        }
        catch (SharedMemoryObjectException&) {
            if (firstIsFail) { // 两次失败视为失败
                boost::diagnostic_information(boost::current_exception());
                break;
            }
            if (version != getLatestVersion()) {
                firstIsFail = true;
                continue;
            }
            else {
                boost::diagnostic_information(boost::current_exception());
                break;
            }
        }
    } while (true);
    return std::shared_ptr<SharedMemoryObject>();
}

inline uint32_t SharedMemoryClientManager::getLatestVersion() const
{
    return *latestShdMemVersion;
}

std::shared_ptr<IEnvChange> SharedMemoryClientManager::getEnvChange(const std::wstring& processFullPath)
{
    if (!contain()) {
        return std::shared_ptr<IEnvChange>();
    }
    if (processFullPath.length() == 0) {
        return std::shared_ptr<IEnvChange>();
    }
    std::shared_ptr<SharedMemoryObject> subMem = getSubSharedMem();
    if (!subMem) {
        return std::shared_ptr<IEnvChange>();
    }
    std::shared_ptr<SharedMemoryObject::ObjectType> obj = subMem->getObj().lock();
    SharedMemoryObject::ObjectType::iterator i = obj->find(*(subMem->getString(processFullPath)));
    if (i == obj->cend()) {
        return std::shared_ptr<IEnvChange>();
    }
    std::shared_ptr<SharedMemoryObject::MapType> map(&(i->second), [](SharedMemoryObject::MapType*) {});
    return std::static_pointer_cast<IEnvChange>(std::make_shared<EnvChange>(subMem, map));
}
