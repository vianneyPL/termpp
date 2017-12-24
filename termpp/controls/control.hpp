#pragma once

#include "../keys.hpp"
#include <termpp/utils/brigand_wrapper.hpp>
#include <termpp/utils/function_traits.hpp>
#include <boost/callable_traits.hpp>
#include <cstring>
#include <exception>
#include <functional>
#include <type_traits>

namespace trm
{
template <typename Function>
struct control
{
    using function_type = Function;

    constexpr control() = delete;
    constexpr control(keys::key_type k, function_type f)
        : _key{k}
        , _function{f}
    {}

    const keys::key_type _key;
    const function_type _function;
};

template <>
struct control<void>
{
    using function_type = void;
};

using empty_control = control<void>;

template <typename T>
struct is_control
{
    using type                  = std::false_type;
    static constexpr bool value = false;
};

template <typename T>
struct is_control<control<T>>
{
    using type                  = std::true_type;
    static constexpr bool value = true;
};

template <typename T>
inline constexpr bool is_control_v = is_control<std::decay_t<T>>::value;

} // namespace trm