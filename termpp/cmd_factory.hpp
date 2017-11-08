#pragma once

#define BRIGAND_NO_BOOST_SUPPORT

#include <brigand.hpp>
#include <exception>
#include <map>
#include <string>
#include <tuple>
#include <variant>

namespace brigand
{
template <typename... T>
using variant_wrapper = typename std::variant<T...>;
template <typename L>
using as_variant = wrap<L, variant_wrapper>;
}

namespace trm
{

class unknown_command : public std::runtime_error
{
public:
    explicit unknown_command()
        : std::runtime_error{"Unknown command"}
    {}
};

template <typename... Args>
class cmd_factory
{
    using list_commands = brigand::list<Args...>;
    using any_command   = brigand::as_variant<list_commands>;

    std::map<std::string, any_command> _constructors;

    template <std::size_t... I>
    void initialize(std::tuple<Args...> commands, std::index_sequence<I...>)
    {
        (initialize_index<I>(commands), ...);
    }

    template <std::size_t I>
    void initialize_index(std::tuple<Args...> commands)
    {
        const auto & cmd = std::get<I>(commands);
        _constructors.insert(std::pair(cmd.name(), cmd));
    }

public:
    cmd_factory(Args... args)
    {
        initialize(std::make_tuple(args...), std::make_index_sequence<sizeof...(Args)>{});
    }

    const any_command & get(const std::string & command_name) const
    {
        if (!_constructors.count(command_name)) throw unknown_command{};
        return _constructors.at(command_name);
    }
};
}