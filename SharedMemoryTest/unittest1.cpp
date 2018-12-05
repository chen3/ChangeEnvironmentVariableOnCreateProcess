#include "stdafx.h"

#include "CppUnitTest.h"
#include "ProcessEnvironment.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using mx404::ChangeEnvironmentVariableOnCreateProcess::IProcessEnvironment;
using mx404::ChangeEnvironmentVariableOnCreateProcess::ProcessEnvironment;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemData;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::IEnvChange;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryClientManager;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryServerManager;

namespace SharedMemoryTest
{		
	TEST_CLASS(SharedMemoryTest)
	{
	public:
		
		TEST_METHOD(TestNormalFlow)
		{
            std::string name = "dddddd";
            std::wstring path = L"bababab";

            SharedMemoryClientManager clientManager(name);
            Assert::IsFalse(clientManager.contain());
            Assert::AreEqual(clientManager.getLatestVersion(), static_cast<uint32_t>(0));
            Assert::IsTrue(clientManager.getEnvChange(L"") == nullptr);
            Assert::IsTrue(clientManager.getEnvChange(path) == nullptr);

            SharedMemoryServerManager serverManager(name);
            Assert::AreEqual(serverManager.getLatestVersion(), static_cast<uint32_t>(0));

            std::shared_ptr<MemData> memData = std::make_shared<MemData>();
            std::unordered_map<std::wstring, std::wstring> map;
            map.insert(std::pair<std::wstring, std::wstring>(L"path", L"c:\\ddd"));

            memData->add(path, map);
            serverManager.updateData(memData);

            Assert::AreEqual(serverManager.getLatestVersion(), static_cast<uint32_t>(1));

            Assert::IsTrue(clientManager.contain());
            Assert::AreEqual(clientManager.getLatestVersion(), static_cast<uint32_t>(1));

            std::shared_ptr<IEnvChange> envChange = clientManager.getEnvChange(path);
            Assert::IsTrue(envChange != nullptr);

            std::shared_ptr<ProcessEnvironment> env = std::make_shared<ProcessEnvironment>();
            std::shared_ptr<IProcessEnvironment> newEnv = envChange->change(env);
            Assert::IsTrue(newEnv != nullptr);
		}

	};
}