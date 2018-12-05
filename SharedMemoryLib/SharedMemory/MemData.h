#pragma once

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {
        namespace SharedMemory {

            class SharedMemoryObject;

            struct MemData
            {
            public:
                MemData();
                using KeyType = std::wstring;
                using ValueType = std::unordered_map<std::wstring, std::wstring>;
                void add(KeyType path, ValueType env);

            private:
                std::unordered_map<KeyType, ValueType> data;
                friend class SharedMemoryObject;
            };
        }
    }
}
