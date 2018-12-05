#pragma once

#include "SharedMemoryException.hpp"

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {
        namespace SharedMemory {
            struct SharedMemoryServerManagerException : virtual SharedMemoryException
            {
            };
        }
    }
}
