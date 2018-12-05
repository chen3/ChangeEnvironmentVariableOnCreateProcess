#pragma once

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {
        namespace SharedMemory {
            struct SharedMemoryException : virtual boost::exception, virtual std::exception
            {
            public:
                using ExceptionInfoString = boost::error_info<struct tag_info_string, std::string>;
            };
        }
    }
}
