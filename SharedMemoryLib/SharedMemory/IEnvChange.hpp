#pragma once

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {

        class IProcessEnvironment;

        namespace SharedMemory {
            class IEnvChange
            {
            public:
                virtual std::shared_ptr<IProcessEnvironment> change(const std::shared_ptr<IProcessEnvironment> env) const = 0;
            };
        }
    }
}
