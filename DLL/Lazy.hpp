// https://blog.csdn.net/gx864102252/article/details/79418152
#pragma once
#include "stdafx.h"

namespace ChangeEnvironmentVariableOnCreateProcess {
    template<typename T>
    class Lazy
    {
    public:
        Lazy(std::function<T()> func)
        {
            m_function = func;
        }

        //延迟执行，将结果保存起来
        T& value()
        {
            if (!m_isCreate)
            {
                m_isCreate = true;
                m_result = m_function();
            }
            return m_result;
        }

    private:
        std::function<T()> m_function;
        T                  m_result;
        bool               m_isCreate = false;
    };
}