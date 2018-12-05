#pragma once

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {
        namespace SharedMemory {

            struct MemData;
            class IEnvChange;
            class SharedMemoryObject;

            class SharedMemoryServerManager
            {
            public:
                SharedMemoryServerManager(const std::string& name);

                inline uint32_t getLatestVersion() const;

                void updateData(std::shared_ptr<MemData> data);

            private:
                std::string name;
                std::shared_ptr<boost::interprocess::windows_shared_memory> mainShdMem;
                std::shared_ptr<boost::interprocess::mapped_region> mainRegion;

                std::shared_ptr<SharedMemoryObject> subSharedMem;

                inline void setLatestVersion(uint32_t value);

                uint32_t* latestShdMemVersion;
            };
        }
    }
}

