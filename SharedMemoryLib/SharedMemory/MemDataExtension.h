#pragma once

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {
        namespace SharedMemory {

            struct MemData;

            class MemDataExtension abstract sealed
            {
            public:
                static unsigned int getSugeestMemoryByteSize(std::shared_ptr<MemData> data);

            private:
                // https://stackoverflow.com/questions/47977829/generate-a-random-string-in-c11
                static std::string random_string();
            };
        }
    }
}
