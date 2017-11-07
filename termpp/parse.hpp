#pragma once

#include <string>
#include <type_traits>

namespace termpp
{
template <typename T>
struct arg;

template <typename T, typename U = std::remove_const_t<std::remove_reference_t<T>>, typename Arg = arg<U>>
struct arg_impl
{
    static U parse(const std::string & value)
    {
        return Arg::parse(value);
    }
    static const char * signature() noexcept
    {
        return Arg::signature();
    }
};

template <>
struct arg<int>
{
    static auto parse(const std::string & arg)
    {
        return std::stoi(arg);
    }
    static const char * signature() noexcept
    {
        return "int";
    }
};

template <>
struct arg<long>
{
    static long parse(const std::string & arg)
    {
        return std::stol(arg);
    }
    static const char * signature() noexcept
    {
        return "long int";
    }
};

template <>
struct arg<long long>
{
    static auto parse(const std::string & arg)
    {
        return std::stoll(arg);
    }
    static const char * signature() noexcept
    {
        return "long long int";
    }
};

template <>
struct arg<unsigned long>
{
    static auto parse(const std::string & arg)
    {
        return std::stoul(arg);
    }
    static const char * signature() noexcept
    {
        return "unsigned long int";
    }
};

template <>
struct arg<unsigned long long>
{
    static auto parse(const std::string & arg)
    {
        return std::stoull(arg);
    }
    static const char * signature() noexcept
    {
        return "unsigned long long int";
    }
};

template <>
struct arg<float>
{
    static auto parse(const std::string & arg)
    {
        return std::stof(arg);
    }
    static const char * signature() noexcept
    {
        return "float";
    }
};

template <>
struct arg<double>
{
    static auto parse(const std::string & arg)
    {
        return std::stod(arg);
    }
    static const char * signature() noexcept
    {
        return "double";
    }
};

template <>
struct arg<long double>
{
    static long double parse(const std::string & arg)
    {
        return std::stold(arg);
    }
    static const char * signature() noexcept
    {
        return "long double";
    }
};

template <>
struct arg<std::string>
{
    static auto parse(const std::string & arg)
    {
        return std::string{arg};
    }
    static const char * signature() noexcept
    {
        return "std::string";
    }
};

template <>
struct arg<char *>
{
    static auto parse(const std::string & arg)
    {
        return arg.c_str();
    }
    static const char * signature() noexcept
    {
        return "char *";
    }
};
}