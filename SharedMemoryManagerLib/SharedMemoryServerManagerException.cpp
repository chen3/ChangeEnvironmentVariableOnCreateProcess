#include "stdafx.h"
#include "SharedMemoryServerManagerException.h"

using mx404::ChangeEnvironmentVariableOnCreateProcess::SharedMemoryManager::SharedMemoryServerManagerException;

SharedMemoryServerManagerException::SharedMemoryServerManagerException()
{
}

SharedMemoryServerManagerException::SharedMemoryServerManagerException(System::String^ message)
    : System::Exception(message)
{
}

SharedMemoryServerManagerException::SharedMemoryServerManagerException(System::String^ message, System::Exception^ innerException)
    : System::Exception(message, innerException)
{
}