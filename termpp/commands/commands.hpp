#pragma once

#include "callback.hpp"
#include "command.hpp"
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <variant>

namespace trm
{

template <typename... Args>
struct commands
{
    using args_list      = brigand::list<empty_command, Args...>;
    using commands_list  = brigand::remove_if<args_list, brigand::not_<is_command<brigand::_1>>>;
    using callbacks_list = brigand::remove_if<args_list, brigand::not_<is_callback<brigand::_1>>>;

    using any_command  = brigand::as_variant<commands_list>;
    using any_callback = brigand::as_variant<callbacks_list>;

private:
    static constexpr inline std::size_t commands_count = brigand::size_t<commands_list>;

public:
    //     commands() = default;
    commands(Args &&... args)
        : _command_map(add_commands(std::make_tuple(args...)))
        , _names(initialize_names(std::make_tuple(args...), std::make_index_sequence<commands_count>{}))
    {}

    std::tuple<std::string, std::error_code> operator()(const std::string & input)
    {
        const auto tokens = internal::split(input, ' ');
        if (tokens.size() == 0)
        {
            return std::tuple{std::string{}, make_error_code(command_errc::empty_input)};
        }
        const auto & command_name = tokens[0];
        if (!std::count(std::begin(_names), std::end(_names), command_name))
        {
            return std::tuple{std::string{}, make_error_code(command_errc::unknown_command)};
        }
        return std::visit(
            [&tokens](const auto & cmd) {
                const auto [args, err] = convert(cmd, tokens);
                if (err) return std::tuple{std::string{}, err};
                return call(cmd, args);
            },
            command);
    }

private:
    const std::unordered_map<std::string, any_command> _command_map;
    const std::array<std::string, commands_count> _names;
    // std::unordered_set<any_callback> _callback_set;

    template <typename Cmd, typename Args, std::size_t... I>
    std::string call(Cmd cmd, Cmd::args_type args, std::index_sequence<I...>)
    {
        return std::invoke(cmd, std::get<I>(args)...);
    }

    std::unordered_map<std::string, any_command> add_commands(std::tuple<Args...> args)
    {
        std::unordered_map<std::string, any_command> cmds;
        std::apply(
            [&cmds](auto arg) {
                if constexpr (is_command_v<Arg>)
                {
                    cmds.insert({arg.name(), arg});
                }
            },
            args);
        return cmds;
    }

    template <std::size_t... I>
    const std::array<std::string, commands_count> initialize_names(std::tuple<Args...> commands, std::index_sequence<I...>) const
    {
        return internal::make_array(std::string{std::get<I>(commands).name()}...);
    }
};
} // namespace trm