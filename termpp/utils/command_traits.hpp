#pragma once

namespace trm
{
template <typename F>
struct cmd;
namespace internal
{
template <typename T>
struct is_cmd
{
    static constexpr bool value = false;
};

template <typename T>
struct is_cmd<cmd<T>>
{
    static constexpr bool value = true;
};

template <typename T>
inline constexpr bool is_cmd_v = is_cmd<T>::value;
}
}
