#pragma once

namespace ChangeEnvironmentVariableOnCreateProcess {
    class ProcessEnvironment
    {
    public:
        ProcessEnvironment(const ProcessEnvironment &) = delete;
        ProcessEnvironment & operator=(const ProcessEnvironment &) = delete;
        ProcessEnvironment & operator=(ProcessEnvironment &&) = delete;

        ProcessEnvironment() noexcept;
        ProcessEnvironment(const ProcessEnvironment && env) noexcept;
        static ProcessEnvironment getFormCurrentProcess();
        static ProcessEnvironment getFormProcessString(const std::shared_ptr<wchar_t> env);
        std::wstring toWinAPINeedString() const noexcept;
        std::wstring& operator [] (const std::wstring& key) noexcept;
        bool contains(const std::wstring& key) const noexcept;
        nlohmann::json toJson() const noexcept;

    private:
        std::unordered_map<std::wstring, std::wstring> env;
        static std::vector<std::wstring> splitString(const std::wstring& s, const std::wstring& c) noexcept;
    };
}
