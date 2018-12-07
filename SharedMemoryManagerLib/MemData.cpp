#include "stdafx.h"
#include "MemData.h"

using UnmanagerMemData = mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemData;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemoryManager::MemData;
using System::Collections::Generic::Dictionary;
using System::Collections::Generic::IDictionary;
using msclr::interop::marshal_as;

MemData::MemData()
    : dictionary(gcnew Dictionary<System::String^, IDictionary<System::String^, System::String^>^>())
{
}

void MemData::Add(System::String ^ path, IDictionary<System::String^, System::String^>^ env)
{
    if (System::String::IsNullOrWhiteSpace(path)) {
        throw gcnew System::ArgumentException("must not be null or empty", "path");
    }
    if (env->Count == 0) {
        throw gcnew System::ArgumentException("must not be empty", "env");
    }
    dictionary[path] = env;
}

std::shared_ptr<UnmanagerMemData> MemData::convert()
{
    std::shared_ptr<UnmanagerMemData> data = std::make_shared<UnmanagerMemData>();
    for each(auto i in dictionary) {
        std::wstring path = marshal_as<std::wstring>(i.Key);
        std::unordered_map<std::wstring, std::wstring> map;
        for each(auto env in i.Value) {
            if (System::String::IsNullOrWhiteSpace(env.Key)
                || System::String::IsNullOrWhiteSpace(env.Value)) {
                continue;
            }
            map[marshal_as<std::wstring>(env.Key)] = marshal_as<std::wstring>(env.Value);
        }
        data->add(path, map);
    }
    return data;
}
