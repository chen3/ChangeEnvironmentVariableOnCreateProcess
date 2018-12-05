#include "stdafx.h"
#include "SharedMemoryObject.h"

#include "MemData.h"
#include "SharedMemoryObject.h"
#include "SharedMemoryObjectException.hpp"

using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryObject;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryObjectException;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemData;
using boost::interprocess::managed_windows_shared_memory;

SharedMemoryObject::SharedMemoryObject(const std::string & name)
    try: name(name)
    , memory(std::make_shared<managed_windows_shared_memory>(boost::interprocess::open_only, name.c_str()))
{
    std::pair<ObjectType*, std::size_t> pair = memory->find_no_lock<ObjectType>(keyName.c_str());
    if (pair.first == nullptr) {
        BOOST_THROW_EXCEPTION(SharedMemoryObjectException()
            << SharedMemoryObjectException::ExceptionInfoString("not find \"Object\" key int memory"));
    }
    obj = std::shared_ptr<ObjectType>(pair.first, [](ObjectType*) {});
}
catch (boost::interprocess::interprocess_exception&)
{
    std::stringstream ss;
    ss << "open " << name << " fail";
    BOOST_THROW_EXCEPTION(SharedMemoryObjectException()
        << SharedMemoryObjectException::ExceptionInfoString(ss.str())
        << boost::errinfo_nested_exception(boost::current_exception()));
}

SharedMemoryObject::SharedMemoryObject(const char * name) 
    : SharedMemoryObject(std::string(name))
{
}

SharedMemoryObject::SharedMemoryObject(const std::string name, unsigned int size)
    try : name(name)
    , memory(std::make_shared<managed_windows_shared_memory>(boost::interprocess::create_only, name.c_str(), size))
{
}
catch (boost::interprocess::interprocess_exception&)
{
    std::stringstream ss;
    ss << "create " << name << " fail";
    BOOST_THROW_EXCEPTION(SharedMemoryObjectException()
        << SharedMemoryObjectException::ExceptionInfoString(ss.str())
        << boost::errinfo_nested_exception(boost::current_exception()));
}

const std::string SharedMemoryObject::keyName = "Object";

std::weak_ptr<SharedMemoryObject::ObjectType> SharedMemoryObject::getObj()
{
    return std::weak_ptr<ObjectType>(obj);
}

void SharedMemoryObject::updateData(std::shared_ptr<MemData> data)
{
    auto segmentManager = memory->get_segment_manager();
    SharedMemoryObject::ObjectType* obj = 
        memory->construct<SharedMemoryObject::ObjectType>(keyName.c_str())(segmentManager);
    for (const std::pair<MemData::KeyType, MemData::ValueType>& n : data->data) {
        SharedMemoryObject::String path(n.first.c_str(), segmentManager);
        SharedMemoryObject::MapType envMap(segmentManager);
        for (const std::pair<std::wstring, std::wstring>& env : n.second) {
            SharedMemoryObject::String name(env.first.c_str(), segmentManager);
            SharedMemoryObject::String value(env.second.c_str(), segmentManager);
            envMap.insert(std::pair<SharedMemoryObject::String, SharedMemoryObject::String>(name, value));
        }
        obj->insert(std::pair<SharedMemoryObject::String, SharedMemoryObject::MapType>(path, envMap));
    }
}

managed_windows_shared_memory::size_type SharedMemoryObject::getNumUniqueObjects()
{
    return memory->get_num_unique_objects();
}

unsigned int SharedMemoryObject::getUsedMemory() const
{
    return memory->get_size() - memory->get_free_memory();
}

std::shared_ptr<SharedMemoryObject::String> SharedMemoryObject::getString(const std::wstring & str)
{
    return std::make_shared<SharedMemoryObject::String>(str.c_str(), memory->get_segment_manager());
}
