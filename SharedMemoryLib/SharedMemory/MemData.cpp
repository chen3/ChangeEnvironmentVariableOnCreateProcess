#include "stdafx.h"
#include "MemData.h"

using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemData;

MemData::MemData()
{
}

void MemData::add(MemData::KeyType path, MemData::ValueType env)
{
    if (path.empty()) {
        throw std::invalid_argument("path must not be empty.");
    }
    if (env.empty()) {
        throw std::invalid_argument("env must not be empty.");
    }
    data[path] = env;
}
