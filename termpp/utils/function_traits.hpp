#pragma once

#include <tuple>
#include <type_traits>

namespace termpp
{
namespace internal
{
// function arg count
template <typename F, typename G = std::remove_pointer_t<F>>
struct function_arg_count : public function_arg_count<decltype(&G::operator())>
{
};

template <typename F, typename R, typename... Args>
struct function_arg_count<F, R(Args...)>
{
    static inline constexpr std::size_t value = sizeof...(Args);
};

template <typename F, typename C, typename R, typename... Args>
struct function_arg_count<F, R (C::*)(Args...) const>
{
    static inline constexpr std::size_t value = sizeof...(Args);
};

template <typename F>
inline constexpr std::size_t function_arg_count_v = function_arg_count<F>::value;

// function return type
template <typename F, typename G = std::remove_pointer_t<F>>
struct function_ret : public function_ret<decltype(&G::operator())>
{
};

template <typename F, typename R, typename... Args>
struct function_ret<F, R(Args...)>
{
    using type = R;
};

template <typename F, typename C, typename R, typename... Args>
struct function_ret<F, R (C::*)(Args...) const>
{
    using type = R;
};

template <typename F>
using function_ret_t = typename function_ret<F>::type;

// function argument at index
template <std::size_t I, typename F, typename G = std::remove_pointer_t<F>>
struct function_arg : public function_arg<I, decltype(&G::operator())>
{
};

template <std::size_t I, typename F, typename R, typename... Args>
struct function_arg<I, F, R(Args...)>
{
    using type = std::tuple_element_t<I, std::tuple<Args...>>;
};

template <std::size_t I, typename F, typename C, typename R, typename... Args>
struct function_arg<I, F, R (C::*)(Args...) const>
{
    using type = std::tuple_element_t<I, std::tuple<Args...>>;
};

template <std::size_t I, typename F>
using function_arg_t = typename function_arg<I, F>::type;
}
}