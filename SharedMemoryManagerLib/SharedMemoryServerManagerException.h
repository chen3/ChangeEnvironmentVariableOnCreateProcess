#pragma once

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {
        namespace SharedMemoryManager {
            ref class SharedMemoryServerManagerException : public System::Exception
            {
            public:
                SharedMemoryServerManagerException();
                SharedMemoryServerManagerException(System::String^ message);
                SharedMemoryServerManagerException(System::String^ message, System::Exception^ innerException);
            };
        }
    }
}
