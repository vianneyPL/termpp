#pragma once

#include <termpp/commands/arg.hpp>
#include <termpp/utils/brigand_wrapper.hpp>
#include <termpp/utils/function_traits.hpp>
#include <boost/callable_traits.hpp>
#include <cstring>
#include <exception>
#include <functional>
#include <type_traits>

namespace trm
{
template <typename F>
struct command
{
    using function_type = F;
    using args_type     = boost::callable_traits::args_t<function_type>;

    static constexpr inline std::size_t args_count = std::tuple_size<args_type>::value;

private:
    using Indices = std::make_index_sequence<args_count>;

public:
    constexpr command() = delete;
    constexpr command(const char * name, F f)
        : _name{name}
        , _signature(initialize_signature(Indices{}))
        , _f{f}
    {
        if (std::strcmp(name, "") == 0)
        {
            throw std::runtime_error{"Cannot create command with empty name"};
        }
    }

    const char * name() const noexcept
    {
        return _name;
    }

    const std::vector<std::string> & signature() const
    {
        return _signature;
    }

    template <typename... Args>
    boost::callable_traits::return_type_t<F> operator()(Args &&... args)
    {
        static_assert(std::is_invocable_v<F, Args...>, "cannot call command with these arguments");
        if constexpr (!boost::callable_traits::has_void_return_v<F>)
        {
            return _f(std::forward<Args &&...>(args...));
        }
        _f(std::forward<Args &&...>(args...));
    }

private:
    const char * _name;
    const std::vector<std::string> _signature;
    function_type _f;

    template <std::size_t... I>
    const std::vector<std::string> initialize_signature(std::index_sequence<I...>) const
    {
        return std::vector<std::string>{arg_impl<internal::function_arg_t<I, F>>::signature()...};
    }
};

template <>
struct command<void>
{
    using function_type = void;
};

using empty_command = command<void>;

template <typename T>
struct is_command
{
    using type                  = std::false_type;
    static constexpr bool value = false;
};

template <typename T>
struct is_command<command<T>>
{
    using type                  = std::true_type;
    static constexpr bool value = true;
};

template <typename T>
inline constexpr bool is_command_v = is_command<std::decay_t<T>>::value;

} // namespace trm