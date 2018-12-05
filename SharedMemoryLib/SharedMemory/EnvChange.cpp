#include "stdafx.h"
#include "EnvChange.h"

#include "../IProcessEnvironment.hpp"

using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::EnvChange;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryObject;
using mx404::ChangeEnvironmentVariableOnCreateProcess::IProcessEnvironment;

EnvChange::EnvChange(std::shared_ptr<SharedMemoryObject> lifeCycle, std::shared_ptr<SharedMemoryObject::MapType> map)
    : lifeCycle(lifeCycle)
    , map(map)
{
}

std::shared_ptr<IProcessEnvironment> EnvChange::change(const std::shared_ptr<IProcessEnvironment> env) const
{
    if (map->empty()) {
        return env;
    }
    std::shared_ptr<IProcessEnvironment> newEnv = env->clone();
    for (std::pair<const SharedMemoryObject::String, SharedMemoryObject::String> pair : *map) {
        std::wstring name = tolower(pair.first.data());
        std::wstring value = pair.second.data();
        if (name == std::wstring(L"path")) {
            if (value[value.length() - 1] != L';') {
                value += ';';
            }
            (*newEnv)[name] = value + (*newEnv)[name];
        }
        else {
            (*newEnv)[name] = value;
        }
    }
    return newEnv;
}

std::wstring EnvChange::tolower(std::wstring s)
{
    std::transform(s.begin(), s.end(), s.begin(), std::towlower);
    return s;
}
