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
template <typename F>
struct control
{
    using function_type = F;

public:
    constexpr control() = delete;
    constexpr control(keys::key_type k, F f)
        : _k{k}
        , _f{f}
    {}

    const keys::key_type key() const noexcept
    {
        return _k;
    }

    template <typename... Args>
    boost::callable_traits::return_type_t<F> operator()(Args &&... args)
    {
        static_assert(std::is_invocable_v<F, Args...>, "cannot call control with these arguments");
        if constexpr (!boost::callable_traits::has_void_return_v<F>)
        {
            return _f(std::forward<Args &&...>(args...));
        }
        _f(std::forward<Args &&...>(args...));
    }

    function_type _f;

private:
    const keys::key_type _k;
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