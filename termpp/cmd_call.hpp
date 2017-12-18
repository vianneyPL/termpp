#pragma once

#include <termpp/cmd.hpp>

namespace trm
{

namespace detail
{

template <typename T, std::size_t... I>
bool check_parse_error(T && t, std::index_sequence<I...>)
{
    return (!std::get<1>(std::get<I>(t)) && ...);
}

template <typename Cmd, std::size_t... I>
const std::tuple<std::string, std::error_code> call_impl(Cmd cmd, const std::vector<std::string> & tokens, std::index_sequence<I...>)
{
    using Indices       = std::make_index_sequence<Cmd::size>;
    using function_type = typename Cmd::function_type;

    auto args = std::make_tuple(arg_impl<internal::function_arg_t<I, function_type>>::parse(tokens.at(I + 1))...);
    if (!check_parse_error(args, Indices{}))
    {
        return std::tuple{std::string{}, make_error_code(cmd_errc::parse_error)};
    }
    return std::tuple{std::invoke(*cmd.function(), std::get<0>(std::get<I>(args))...), std::error_code{}};
}
} // namespace detail

template <typename Cmd>
const std::error_code call_check(Cmd cmd, const std::vector<std::string> & tokens)
{
    if (cmd.name() != tokens[0])
    {
        return make_error_code(cmd_errc::wrong_command);
    }
    else if (Cmd::size > (tokens.size() - 1))
    {
        return make_error_code(cmd_errc::not_enough_arguments);
    }
    else if (Cmd::size < (tokens.size() - 1))
    {
        return make_error_code(cmd_errc::too_much_arguments);
    }
    return {};
}

template <typename Cmd>
const std::tuple<std::string, std::error_code> call(Cmd cmd, const std::vector<std::string> & tokens)
{
    using Indices = std::make_index_sequence<Cmd::size>;

    if (auto err = call_check(cmd, tokens); err)
    {
        return std::tuple{std::string{}, err};
    }
    return detail::call_impl(cmd, tokens, Indices{});
}
} // namespace trm