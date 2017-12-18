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
public:
    static constexpr std::size_t size = internal::function_arg_count_v<F>;
    using function_type               = F;

private:
    static constexpr bool is_ret_allowed_v = std::is_convertible_v<internal::function_ret_t<F>, std::string>;
    using Indices                          = std::make_index_sequence<size>;

    static_assert(is_ret_allowed_v, "return type must be convertible to std::string.");

public:
    constexpr cmd(const char * name, F f)
        : _name(name)
        , _function(f)
        , _signature(initialize_signature(Indices{}))
    {}

    constexpr const char * name() const noexcept
    {
        return _name;
    }

    const function_type function() const
    {
        return _function;
    }

    const std::vector<std::string> & signature() const
    {
        return _signature;
    }

private:
    const char * _name;
    const function_type _function;
    const std::vector<std::string> _signature;

    template <std::size_t... I>
    const std::vector<std::string> initialize_signature(std::index_sequence<I...>) const
    {
        return std::vector<std::string>{arg_impl<internal::function_arg_t<I, F>>::signature()...};
    }
};
} // namespace trm
