#pragma once

#include "SharedMemoryException.hpp"

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {
        namespace SharedMemory {
            struct SharedMemoryObjectException : virtual SharedMemoryException
            {
            };
        }
    }
}
