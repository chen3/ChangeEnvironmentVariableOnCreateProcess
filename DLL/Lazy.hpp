// https://blog.csdn.net/gx864102252/article/details/79418152
#include "stdafx.h"

template<typename T>
class Lazy
{
public:
    //保存需要延迟执行的函数
    template<typename Func, typename ...Args>
    Lazy(Func&& f, Args&& ...args)
    {
        m_function = [&f, &args...]() { return f(std::forward<Args>(args)...); };
        m_function = std::bind(f, std::forward<Args>(args)...);
    }

    //延迟执行，将结果保存起来
    T& value()
    {
        if (!m_isCreate)
        {
            m_result = m_function();
            m_isCreate = true;
        }
        return m_result;
    }

private:
    std::function<T()> m_function;
    T                  m_result;
    bool               m_isCreate = false;
};

//帮助函数 -> 将要执行的函数以及函数的入参保存成Lazy对象
template<class Func, typename ...Args>
Lazy<typename std::result_of<Func(Args...)>::type>
    lazy(Func&& f, Args&& ...args)
{
    return Lazy<typename std::result_of<Func(Args...)>::type>(
        std::forward<Func>(f), std::forward<Args>(args)...);
}