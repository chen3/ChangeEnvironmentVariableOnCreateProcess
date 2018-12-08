#pragma once

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {

        namespace SharedMemory {
            struct MemData;
        }

        namespace SharedMemoryManager {
            public ref struct MemData
            {
            public:
                MemData();

                void Add(System::String^ path, System::Collections::Generic::IDictionary<System::String^, System::String^>^ env);

            internal:
                std::shared_ptr<SharedMemory::MemData> convert();

            private:
                System::Collections::Generic::IDictionary<System::String^, 
                    System::Collections::Generic::IDictionary<System::String^, System::String^>^>^ dictionary;
            };
        }
    }
}
