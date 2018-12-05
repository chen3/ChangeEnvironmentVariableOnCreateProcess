#pragma once

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {
        class ProcessEnvironment : public IProcessEnvironment
        {
        public:
            ProcessEnvironment() noexcept;
            ProcessEnvironment(const ProcessEnvironment & penv) noexcept;
            ProcessEnvironment(const ProcessEnvironment && penv) noexcept;

            static ProcessEnvironment getFormCurrentProcess();
            static ProcessEnvironment getFormProcessString(const std::shared_ptr<wchar_t> env);
            virtual std::wstring toWinAPINeedString() const noexcept override;
            virtual std::wstring& operator [] (const std::wstring& key) noexcept override;
            virtual bool contains(const std::wstring& key) const noexcept override;
            virtual std::shared_ptr<IProcessEnvironment> clone() const override;
            nlohmann::json toJson() const noexcept;

        private:
            std::unordered_map<std::wstring, std::wstring> env;
            static std::vector<std::wstring> splitString(const std::wstring& s, const std::wstring& c) noexcept;
        };
    }
}