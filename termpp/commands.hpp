#pragma once

#include <termpp/cmd.hpp>
#include <termpp/errors.hpp>
#include <termpp/utils/command_traits.hpp>
#include <termpp/utils/split.hpp>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace termpp
{
template <typename... Args>
class commands
{
    using commands_type                        = std::tuple<Args...>;
    static constexpr std::size_t commands_size = sizeof...(Args);

    static_assert((internal::is_cmd_v<Args> && ...), "initializer types should be termpp::cmd.");

    // using listed_commands = brigand::list<Args...>;
    // using any_command     = brigand::as_variant<listed_commands>;

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