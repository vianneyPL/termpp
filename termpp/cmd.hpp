#pragma once

#include <termpp/errors.hpp>
#include <termpp/parse.hpp>
#include <termpp/utils/function_traits.hpp>
#include <termpp/utils/length.hpp>
#include <termpp/utils/make_array.hpp>
#include <functional>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace trm
{
template <typename F>
class cmd
{
    static constexpr std::size_t cmd_size  = internal::function_arg_count_v<F>;
    static constexpr bool is_ret_allowed_v = std::is_convertible_v<internal::function_ret_t<F>, std::string>;

    using cmd_type = std::pair<std::string_view, F>;
    using Indices  = std::make_index_sequence<cmd_size>;

    static_assert(is_ret_allowed_v, "return type must be convertible to std::string.");

public:
    constexpr cmd(const char * name, F f)
        : _name(name)
        , _cmd(std::string_view{name, internal::length(name)}, f)
        , _signature(initialize_signature(Indices{}))
    {}

    auto call(std::vector<std::string> tokens) const
    {
        if (_cmd.first != tokens[0])
        {
            return std::tuple{std::string{}, make_error_code(cmd_errc::wrong_command)};
        }
        else if (cmd_size > (tokens.size() - 1))
        {
            return std::tuple{std::string{}, make_error_code(cmd_errc::not_enough_arguments)};
        }
        else if (cmd_size < (tokens.size() - 1))
        {
            return std::tuple{std::string{}, make_error_code(cmd_errc::too_much_arguments)};
        }
        return std::tuple{call_impl(tokens, Indices{}), std::error_code{}};
    }

    constexpr const char * name() const noexcept
    {
        return _name;
    }

    const std::vector<std::string> & signature() const
    {
        return _signature;
    }

private:
    cmd_type _cmd;
    const char * _name;
    const std::vector<std::string> _signature;

    template <std::size_t... I>
    std::string call_impl(const std::vector<std::string> & tokens, std::index_sequence<I...>) const
    {
        return std::invoke(*_cmd.second, arg_impl<internal::function_arg_t<I, F>>::parse(tokens.at(I + 1))...);
    }

    template <std::size_t... I>
    auto initialize_signature(std::index_sequence<I...>) const
    {
        return std::vector<std::string>{arg_impl<internal::function_arg_t<I, F>>::signature()...};
    }
};
} // namespace trm
