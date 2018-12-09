#pragma once

namespace mx404 {
    namespace ChangeEnvironmentVariableOnCreateProcess {
        namespace SharedMemory {

            struct MemData;
            class MemDataExtension;
            class SharedMemoryServerManager;

            class SharedMemoryObject
            {
            public:
                SharedMemoryObject(const std::string& name);

                SharedMemoryObject(const char *name);

                template<class T>
                using Alloc = boost::interprocess::allocator<T, boost::interprocess::managed_windows_shared_memory::segment_manager>;
                using String = boost::interprocess::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc<wchar_t>>;
                using MapType = std::unordered_map<String, String, boost::hash<String>, std::equal_to<String>, Alloc<std::pair<const String, String>>>;
                using ObjectType = std::unordered_map<String, MapType, boost::hash<String>, std::equal_to<String>, Alloc<std::pair<const String, MapType>>>;

                std::weak_ptr<ObjectType> getObj();
                uint32_t getUsedMemory() const;

                std::shared_ptr<String> getString(const std::wstring& str);
            private:
                std::string name;
                std::shared_ptr<boost::interprocess::managed_windows_shared_memory> memory;
                std::shared_ptr<ObjectType> obj;
                static const std::string keyName;

                friend class SharedMemoryServerManager;
                friend class MemDataExtension;
                SharedMemoryObject(const std::string name, uint32_t size);
                void updateData(std::shared_ptr<MemData> data);
                boost::interprocess::managed_windows_shared_memory::size_type getNumUniqueObjects();
            };
        }
    }
}