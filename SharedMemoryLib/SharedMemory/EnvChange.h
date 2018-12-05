#pragma once

#include "IEnvChange.hpp"
#include "SharedMemoryObject.h"

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {

        class IProcessEnvironment;

        namespace SharedMemory {
            class EnvChange : public IEnvChange
            {
            public:
                EnvChange(std::shared_ptr<SharedMemoryObject> lifeCycle,
                    std::shared_ptr<SharedMemoryObject::MapType> map);

                // Inherited via IEnvChange
                virtual std::shared_ptr<IProcessEnvironment> change(const std::shared_ptr<IProcessEnvironment> env) const override;

            private:
                std::shared_ptr<SharedMemoryObject> lifeCycle;
                std::shared_ptr<SharedMemoryObject::MapType> map;

                static std::wstring tolower(std::wstring s);
            };
        }
    }
}


