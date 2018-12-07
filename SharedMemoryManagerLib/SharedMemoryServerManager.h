#pragma once

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {

        namespace SharedMemory {
            class SharedMemoryServerManager;
        }

        namespace SharedMemoryManager {

            ref struct MemData;

            ref class SharedMemoryServerManager
            {
            public:
                SharedMemoryServerManager(System::String^ name);
                ~SharedMemoryServerManager();

                inline System::UInt32 GetLatestVersion();

                void UpdateData(MemData^ data);

            private:
                SharedMemory::SharedMemoryServerManager* manager;
            };
        }
    }
}
