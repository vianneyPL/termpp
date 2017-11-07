#pragma once

#include <termpp/errors.hpp>
#include <termpp/parse.hpp>
#include <termpp/utils/command_traits.hpp>
#include <termpp/utils/function_traits.hpp>
#include <termpp/utils/length.hpp>
#include <termpp/utils/make_array.hpp>
#include <termpp/utils/split.hpp>
#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace termpp
{
template <typename F>
class cmd
{
    using cmd_type                        = std::pair<std::string_view, F>;
    static constexpr std::size_t cmd_size = internal::function_arg_count_v<F>;
    static_assert(std::is_convertible_v<internal::function_ret_t<F>, std::string>, "return type must be convertible to std::string.");
    // static_assert(std::is_function_v<std::remove_pointer_t<F>>, "second argument must be a function.");

public:
    constexpr cmd(std::string_view name, F f)
        : cmd_name(name.data())
        , _cmd(name, f)
    {}
    constexpr cmd(const char * name, F f)
        : cmd_name(name)
        , _cmd(std::string_view{name, internal::length(name)}, f)
    {}

    auto parser(std::vector<std::string> tokens) const
    {
        if (_cmd.first != tokens[0])
        {
            return std::make_tuple(std::string{}, make_error_code(cmd_errc::wrong_command));
        }
        else if (cmd_size > (tokens.size() - 1))
        {
            return std::make_tuple(std::string{}, make_error_code(cmd_errc::not_enough_arguments));
        }
        else if (cmd_size < (tokens.size() - 1))
        {
            return std::make_tuple(std::string{}, make_error_code(cmd_errc::too_much_arguments));
        }
        return std::make_tuple(parser_impl(tokens), std::error_code{});
    }

    constexpr const char * name() const noexcept
    {
        return _cmd.first.data();
    }

    const char * cmd_name;

private:
    cmd_type _cmd;

    template <typename Indices = std::make_index_sequence<cmd_size>>
    std::string parser_impl(const std::vector<std::string> & tokens) const
    {
        return parser_impl_at_index(tokens, Indices{});
    }

    template <std::size_t... I>
    std::string parser_impl_at_index(const std::vector<std::string> & tokens, std::index_sequence<I...>) const
    {
        return std::invoke(*_cmd.second, arg_impl<internal::function_arg_t<I, F>>::parse(tokens.at(I + 1))...);
    }
};

template <typename... Args>
class commands
{
    using commands_type                        = std::tuple<Args...>;
    static constexpr std::size_t commands_size = sizeof...(Args);

    static_assert((internal::is_cmd_v<Args> && ...), "initializer type should be a termpp::cmd.");

public:
    constexpr commands(Args... args)
        : _cmds(std::make_tuple(args...))
    {}

    auto call(std::string cmd_line) const
    {
        const auto tokens = internal::split(cmd_line, ' ');
        if (tokens.size() == 0)
        {
            return std::make_tuple(std::string{}, make_error_code(cmd_errc::empty_input));
        }
        return call_impl(tokens, std::make_index_sequence<commands_size>{});
    }

private:
    commands_type _cmds;

    template <std::size_t... I>
    auto call_impl(const std::vector<std::string> & tokens, std::index_sequence<I...>) const
    {
        std::string result;
        bool success = (call_impl_at_index<I>(tokens, result) | ...);
        if (!success)
        {
            return std::make_tuple(std::string{}, make_error_code(cmd_errc::no_parser_found));
        }
        return std::make_tuple(result, std::error_code{});
    }

    template <std::size_t I>
    bool call_impl_at_index(const std::vector<std::string> & tokens, std::string & result) const
    {
        auto e         = std::get<I>(_cmds);
        auto[r, error] = e.parser(tokens);
        if (!error)
        {
            result = r;
        }
        return (!error);
    }
};
}