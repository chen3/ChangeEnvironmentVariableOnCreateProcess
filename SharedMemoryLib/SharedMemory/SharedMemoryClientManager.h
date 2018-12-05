#pragma once

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {
        namespace SharedMemory {

            struct MemData;
            class IEnvChange;
            class SharedMemoryObject;

            class SharedMemoryClientManager
            {
            public:
                SharedMemoryClientManager(const std::string& name);

                bool contain();

                std::shared_ptr<IEnvChange> getEnvChange(const std::wstring& processFullPath);

                inline uint32_t getLatestVersion() const;

            private:
                std::string name;
                std::shared_ptr<boost::interprocess::windows_shared_memory> mainShdMem;
                std::shared_ptr<boost::interprocess::mapped_region> mainRegion;

                std::shared_ptr<SharedMemoryObject> getSubSharedMem();

                uint32_t* latestShdMemVersion;
            };
        }
    }
}

