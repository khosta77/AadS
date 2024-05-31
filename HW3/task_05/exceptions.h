#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <exception>
#include <string>
#include <cxxabi.h>
#include <typeinfo>

#include "IGraph.h"

class IGraphException : public std::exception
{
private:
    std::string m_msg;

public:
    explicit IGraphException(const std::string &msg) : m_msg(msg) {}
    const char *what() const noexcept override
    {
		return m_msg.c_str();
	}
};

class OutOfRangeFromTo : public IGraphException
{
public:
    explicit OutOfRangeFromTo( const int& from, const int& to, const int& size ) : IGraphException(
            "Индекс для " + ( ( from < size ) ? ( "from(" + std::to_string(from) + ") " ) : ( 
            "to(" + std::to_string(to) + ") " ) ) + " больше чем " + std::to_string(size)
    ) {}
};

class OutOfRangeVertex : public IGraphException
{
public:
    explicit OutOfRangeVertex( const int& vertex, const int& count ) : IGraphException(
            "Индекс " + std::to_string(vertex) + " больше чем " + std::to_string(count)
    ) {}
};

class MethodException : public IGraphException
{
public:
    explicit MethodException( IGraph& g, const std::string& method ) : IGraphException(
            "Ошибка в объекте класса " + std::string(abi::__cxa_demangle(typeid(g).name(), nullptr,
            nullptr, nullptr)) + " метод " + method
    ) {}
};

class FileNotOpen : public std::exception
{
public:
    explicit FileNotOpen(const std::string &msg) : m_msg(msg) {}
    const char *what() const noexcept override { return m_msg.c_str(); }
private:
    std::string m_msg;
};

#endif  // _EXCEPTIONS_H_
