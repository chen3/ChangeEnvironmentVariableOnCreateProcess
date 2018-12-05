#pragma once

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {
        class IProcessEnvironment {
        public:
            virtual std::wstring& operator [] (const std::wstring& key) noexcept = 0;
            virtual bool contains(const std::wstring& key) const noexcept = 0;
            virtual std::shared_ptr<IProcessEnvironment> clone() const = 0;
            virtual std::wstring toWinAPINeedString() const noexcept = 0;
        };
    }
}