#pragma once

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {
        namespace SharedMemory {

            struct MemData;

            class MemDataExtension abstract sealed
            {
            public:
                static uint32_t getSugeestMemoryByteSize(std::shared_ptr<MemData> data);
            };
        }
    }
}
