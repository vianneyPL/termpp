#pragma once

#include <termpp/cmd.hpp>
#include <termpp/cmd_factory.hpp>
#include <termpp/errors.hpp>
#include <termpp/utils/command_traits.hpp>
#include <termpp/utils/split.hpp>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace trm
{

struct command_visitor
{
    command_visitor(std::vector<std::string> tokens)
        : _tokens{tokens}
    {}

    template <typename Command>
    auto operator()(const Command & cmd)
    {
        return cmd.parser(_tokens);
    }

private:
    std::vector<std::string> _tokens;
};

template <typename... Args>
class commands
{
    static_assert((internal::is_cmd_v<Args> && ...), "initializer types should be trm::cmd.");

public:
    constexpr commands(Args... args)
        : _cmd_factory(args...)
    {}

    auto call(std::string cmd_line) const
    {
        const auto tokens = internal::split(cmd_line, ' ');
        if (tokens.size() == 0)
        {
            return std::make_tuple(std::string{}, make_error_code(cmd_errc::empty_input));
        }
        std::tuple<std::string, std::error_code> result;
        try
        {
            const auto & cmd = _cmd_factory.get(tokens[0]);
            result           = std::visit(command_visitor(tokens), cmd);
        }
        catch (const std::runtime_error & /*unused*/)
        {
            result = std::make_tuple(std::string{}, make_error_code(cmd_errc::unknown_command));
        }
        return result;
    }

private:
    cmd_factory<Args...> _cmd_factory;
};
}