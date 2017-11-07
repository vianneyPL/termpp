#pragma once

#include <termpp/errors.hpp>
#include <termpp/parse.hpp>
#include <termpp/utils/function_traits.hpp>
#include <termpp/utils/length.hpp>
#include <functional>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace termpp
{
template <typename F>
class cmd
{
    using cmd_type = std::pair<std::string_view, F>;

    static constexpr std::size_t cmd_size  = internal::function_arg_count_v<F>;
    static constexpr bool is_ret_allowed_v = std::is_convertible_v<internal::function_ret_t<F>, std::string>;

    static_assert(is_ret_allowed_v, "return type must be convertible to std::string.");

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
} // namespace termpp