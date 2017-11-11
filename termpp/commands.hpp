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

    const auto call(std::string cmd_line) const
    {
        const auto tokens = internal::split(cmd_line, ' ');
        if (auto err = call_check(tokens); err)
        {
            return std::tuple{std::string{}, err};
        }
        const auto & command_name = tokens[0];
        const auto & command      = _cmd_factory.get(command_name);
        return std::visit([&tokens](const auto & cmd) { return cmd.call(tokens); }, command);
    }

    const std::array<const std::string, cmds_size> & names() const
    {
        return _names;
    }

    const std::tuple<std::vector<std::string>, std::error_code> signature(const std::string & name)
    {
        try
        {
            const auto & cmd = _cmd_factory.get(name);
            return std::make_tuple(std::visit([](const auto & cmd) { return cmd.signature(); }, cmd), std::error_code{});
        }
        catch (const std::runtime_error & /*unused*/)
        {
            return std::make_tuple(std::vector<std::string>{}, make_error_code(cmd_errc::unknown_command));
        }
    }

private:
    const cmd_factory<Args...> _cmd_factory;
    const std::array<std::string, cmds_size> _names;

    const std::error_code call_check(const std::vector<std::string> & tokens) const
    {
        if (tokens.size() == 0)
        {
            return make_error_code(cmd_errc::empty_input);
        }
        const auto & command_name = tokens[0];
        if (!std::count(std::begin(_names), std::end(_names), command_name))
        {
            return make_error_code(cmd_errc::unknown_command);
        }
        return {};
    }

    template <std::size_t... I>
    const std::array<std::string, cmds_size> initialize_names(std::tuple<Args...> commands, std::index_sequence<I...>) const
    {
        return internal::make_array(std::string{std::get<I>(commands).name()}...);
    }
};
}
