#include "stdafx.h"
#include "MemDataExtension.h"

#include "MemData.h"
#include "MemDataTooLargeException.hpp"
#include "SharedMemoryObject.h"

using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemData;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemDataExtension;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemDataTooLargeException;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryObject;
using boost::interprocess::managed_windows_shared_memory;

unsigned int MemDataExtension::getSugeestMemoryByteSize(std::shared_ptr<MemData> data)
{
    const int maxCount = 6; // 最多使用内存为 maxCount * 5M
    const boost::ulong_long_type fiveM = 5 * 1024 * 1024;
    for (int i = 1; i <= maxCount; ++i) {
        try {
            unsigned int size = i * fiveM;  // 每次增加5兆
            SharedMemoryObject obj(random_string(), size);  // 可以将随机字符串改为使用uuid
            obj.updateData(data);
            const int hypothesisAligmentByte = 8;
            unsigned int needSize =
                (obj.getUsedMemory() / hypothesisAligmentByte + obj.getNumUniqueObjects()) * hypothesisAligmentByte;
            return needSize * 2;
        }
        catch (boost::interprocess::bad_alloc&) {   // 内存不足则继续增加
            continue;
        }
        catch (boost::interprocess::interprocess_exception& ex) {   // 同名共享内存碰到几率极低
            boost::diagnostic_information(ex);
            continue;
        }
    }
    auto byteSize = maxCount * fiveM;
    BOOST_THROW_EXCEPTION(MemDataTooLargeException()
        << MemDataTooLargeException::ExceptionInfoString("MemData greate than " + std::to_string(byteSize) + " byte"));
}

std::string mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemDataExtension::random_string()
{
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(str.begin(), str.end(), generator);

    return str.substr(0, 32);    // assumes 32 < number of characters in str
}
