#pragma once

#include <tuple>
#include <cassert>
#include <string_view>
#include <type_traits>
#include "parse.hpp"

namespace term
{
    template <typename F>
    class cmd
    {
        using cmd_type = std::pair<std::string_view, F>;

    public:
        constexpr cmd(std::string_view name, F f) : _cmd(name, f) {}
        constexpr cmd(const char *name, F f) : _cmd(std::string_view{name, internal::length(name)}, f) {}


        void parser(std::vector<std::string> tokens)
        {
            if (_cmd.first == tokens[0])
            {
                parse_impl(tokens, *_cmd.second);
            }
        }

    private:
        cmd_type _cmd;

        template<typename R, typename ...Args>
        void parse_impl(const std::vector<std::string> & tokens, R (*f)(Args...))
        {
            parser_impl<sizeof...(Args)>(tokens);
        }
        
        template<std::size_t N, typename Indices = std::make_index_sequence<N>>
        void parser_impl(const std::vector<std::string> & tokens)
        {
            parser_impl_at_index(tokens, Indices{});
        }
        
        template <std::size_t... I>
        void parser_impl_at_index(const std::vector<std::string> & tokens, std::index_sequence<I...>)
        {
            (*_cmd.second)(parse<decltype(internal::get_function_arg_type<I>(*_cmd.second))>(tokens.at(I + 1))...);
        }
    };

    template <typename ...Args>
    class commands
    {
        using commands_type = std::tuple<Args...>;
        static constexpr std::size_t commands_size = sizeof...(Args);

    public:
        constexpr commands(Args&&... args) : _cmds(std::make_tuple(args...))
        {
            // static_assert(std::is_same_v<std::common_type_t<Args...>, cmd>, "All arg should be of type std::pair<std::string_view, function>.");
        }

        void call(std::string cmd_line)
        {
            const auto tokens = internal::split(cmd_line, ' ');
            assert(tokens.size() > 0);
            call_impl(tokens, std::make_index_sequence<commands_size>{});
        }

    private:
        commands_type _cmds;

        template <std::size_t... I>
        void call_impl(const std::vector<std::string> & tokens, std::index_sequence<I...>)
        {
            (call_impl_at_index<I>(tokens), ...);
        }

        template <std::size_t I>
        void call_impl_at_index(const std::vector<std::string> & tokens)
        {
            auto e = std::get<I>(_cmds);
            e.parser(tokens);
        }
    };

//     template <typename ...Args>
//     auto make_commands(Args... args)
//     {
//         return commands<std::pair<Args>(args...);
//     }
}