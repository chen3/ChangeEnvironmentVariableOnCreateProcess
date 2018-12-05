#pragma once

#include "SharedMemoryObjectException.hpp"

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {
        namespace SharedMemory {
            struct SharedMemoryObjectCreateException : virtual SharedMemoryObjectException
            {
            };
        }
    }
}
