#pragma once

#include "parse.hpp"
#include <cassert>
#include <termpp/utils/function_traits.hpp>
#include <termpp/utils/command_traits.hpp>
#include <termpp/utils/make_array.hpp>
#include <functional>
#include <iomanip>
#include <iostream>
#include <termpp/utils/length.hpp>
#include <termpp/utils/split.hpp>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace term
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
        : cmd_name(name.data()), _cmd(name, f)
    {}
    constexpr cmd(const char * name, F f)
        : cmd_name(name), _cmd(std::string_view{name, internal::length(name)}, f)
    {}

    std::tuple<std::string, bool> parser(std::vector<std::string> tokens) const
    {
        if ((cmd_size != (tokens.size() - 1)) || (_cmd.first != tokens[0]))
        {
            return std::make_tuple(std::string(""), false);
        }
        return std::make_tuple(parser_impl(tokens), true);
    }

    constexpr const char *name() const noexcept
    {
        return _cmd.first.data();
    }

    const char *cmd_name;

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
        return std::invoke(*_cmd.second, parser_impl_at_index_impl<I>(tokens)...);
    }

    template <std::size_t I>
    auto parser_impl_at_index_impl(const std::vector<std::string> & tokens) const
    {
        using arg_type = internal::function_arg_t<I, F>;
        return parse<arg_type>(tokens.at(I + 1));
    }
};

template <typename... Args>
class commands
{
    using commands_type                        = std::tuple<Args...>;
    static constexpr std::size_t commands_size = sizeof...(Args);

    static_assert((internal::is_cmd_v<Args> && ...), "initializer type should be a term::cmd.");

public:
    constexpr commands(Args ... args)
        : _cmds(std::make_tuple(args...))
    {
    }

    std::string call(std::string cmd_line) const
    {
        const auto tokens = internal::split(cmd_line, ' ');
        assert(tokens.size() > 0);
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
            std::cerr << "No appropriate parser was found." << '\n';
        }
        return result;
    }

    template <std::size_t I>
    bool call_impl_at_index(const std::vector<std::string> & tokens, std::string &result) const
    {
        auto e = std::get<I>(_cmds);
        auto [r, success] = e.parser(tokens);
        if (success)
        {
            result = r;
        }
        return success;
    }
};
}