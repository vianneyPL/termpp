#pragma once

#include <termpp/cmd.hpp>
#include <termpp/cmd_factory.hpp>
#include <termpp/errors.hpp>
#include <termpp/utils/command_traits.hpp>
#include <termpp/utils/make_array.hpp>
#include <termpp/utils/split.hpp>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace trm
{

struct parser_visitor
{
    parser_visitor(std::vector<std::string> tokens)
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

struct signature_visitor
{
    signature_visitor()
    {}
    template <typename Command>
    auto operator()(const Command & cmd) const
    {
        return cmd.signature();
    }
};

template <typename... Args>
class commands
{
    static constexpr std::size_t cmds_size = sizeof...(Args);

    static_assert((internal::is_cmd_v<Args> && ...), "initializer types should be trm::cmd.");

public:
    constexpr commands(Args... args)
        : _cmd_factory(args...)
        , _names(initialize_names(std::make_tuple(args...), std::make_index_sequence<cmds_size>{}))
    {}

    auto call(std::string cmd_line) const
    {
        const auto tokens = internal::split(cmd_line, ' ');
        if (tokens.size() == 0)
        {
            return std::make_tuple(std::string{}, make_error_code(cmd_errc::empty_input));
        }
        try
        {
            const auto & cmd = _cmd_factory.get(tokens[0]);
            return std::visit(parser_visitor(tokens), cmd);
        }
        catch (const std::runtime_error & /*unused*/)
        {
            return std::tuple{std::string{}, make_error_code(cmd_errc::unknown_command)};
        }
    }

    const std::array<std::string, cmds_size> & names() const
    {
        return _names;
    }

    const auto signature(const std::string & name)
    {
        try
        {
            const auto & cmd = _cmd_factory.get(name);
            return std::make_tuple(std::visit(signature_visitor(), cmd), std::error_code{});
        }
        catch (const std::runtime_error & /*unused*/)
        {
            return std::make_tuple(std::vector<std::string>{}, make_error_code(cmd_errc::unknown_command));
        }
    }

private:
    cmd_factory<Args...> _cmd_factory;
    const std::array<std::string, cmds_size> _names;

    template <std::size_t... I>
    auto initialize_names(std::tuple<Args...> commands, std::index_sequence<I...>) const
    {
        return internal::make_array(initialize_names_index<I>(commands)...);
    }

    template <std::size_t I>
    std::string initialize_names_index(std::tuple<Args...> commands) const
    {
        const auto & cmd = std::get<I>(commands);
        return std::string{cmd.name()};
    }
};
}
