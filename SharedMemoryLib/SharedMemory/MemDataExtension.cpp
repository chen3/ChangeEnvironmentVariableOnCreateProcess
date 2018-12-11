#include "stdafx.h"
#include "MemDataExtension.h"

#include "MemData.h"
#include "MemDataTooLargeException.hpp"
#include "SharedMemoryObject.h"
#include "..\Util\StringHelper.h"

using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemData;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemDataExtension;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::MemDataTooLargeException;
using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemory::SharedMemoryObject;
using mx404::Util::random_string;
using boost::interprocess::managed_windows_shared_memory;

uint32_t MemDataExtension::getSugeestMemoryByteSize(std::shared_ptr<MemData> data)
{
    const int maxCount = 6; // 最多使用内存为 maxCount * 5M
    const boost::ulong_long_type fiveM = 5 * 1024 * 1024;
    for (int i = 1; i <= maxCount; ++i) {
        try {
            unsigned int size = i * fiveM;  // 每次增加5兆
            SharedMemoryObject obj(random_string(), size);  // 可以将随机字符串改为使用uuid
            obj.updateData(data);
            const uint32_t hypothesisAligmentByte = 8;   // 返回建议大小为8的倍数，方便对齐
            // 建议大小为实际使用大小的两倍
            return static_cast<uint32_t>(
                std::ceil(obj.getUsedMemory() * 2.0 / hypothesisAligmentByte)
                    * hypothesisAligmentByte);
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