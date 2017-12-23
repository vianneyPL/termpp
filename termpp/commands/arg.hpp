#pragma once

#include <termpp/errors.hpp>
#include <string>
#include <system_error>
#include <tuple>
#include <type_traits>

namespace trm
{
template <typename T>
struct arg;

template <typename T, typename U = std::remove_const_t<std::remove_reference_t<T>>, typename Arg = arg<U>>
struct arg_impl
{
    static std::tuple<U, std::error_code> parse(const std::string & value)
    {
        try
        {
            return Arg::parse(value);
        }
        catch (...)
        {
            // TODO(vianney): see if it's possible to remove the constraint of default constructor on type U
            return std::tuple{U{}, make_error_code(command_errc::parse_error)};
        }
    }
    static std::string signature() noexcept
    {
        return Arg::signature();
    }
};

template <>
struct arg<int>
{
    static auto parse(const std::string & arg)
    {
        return std::tuple{std::stoi(arg), std::error_code{}};
    }
    static const char * signature() noexcept
    {
        return "int";
    }
};

template <>
struct arg<long>
{
    static auto parse(const std::string & arg)
    {
        return std::tuple{std::stol(arg), std::error_code{}};
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
        return std::tuple{std::stoll(arg), std::error_code{}};
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
        return std::tuple{std::stoul(arg), std::error_code{}};
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
        return std::tuple{std::stoull(arg), std::error_code{}};
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
        return std::tuple{std::stof(arg), std::error_code{}};
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
        return std::tuple{std::stod(arg), std::error_code{}};
    }
    static const char * signature() noexcept
    {
        return "double";
    }
};

template <>
struct arg<long double>
{
    static auto parse(const std::string & arg)
    {
        return std::tuple{std::stold(arg), std::error_code{}};
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
        return std::tuple{std::string{arg}, std::error_code{}};
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
        return std::tuple{arg.c_str(), std::error_code{}};
    }
    static const char * signature() noexcept
    {
        return "char *";
    }
};
} // namespace trm