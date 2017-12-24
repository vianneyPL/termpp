#pragma once

#include "control.hpp"
#include <termpp/errors.hpp>
#include <termpp/utils/make_array.hpp>
#include <iostream>
#include <tuple>
#include <unordered_map>

namespace trm
{

struct controls_interface
{
    controls_interface() = default;
    virtual ~controls_interface()
    {}
    virtual std::error_code operator()(const keys::key_type key)
    {
        std::cout << "override didn't work...\n";
    }
    // virtual std::error_code operator()(const keys::key_type key);
};

template <typename... Args>
struct controls : public controls_interface
{
    using args_list     = brigand::list<Args...>;
    using controls_list = brigand::remove_if<args_list, brigand::not_<is_control<brigand::_1>>>;

private:
    template <typename Control>
    struct get_function_type
    {
        using type = typename Control::function_type;
    };

    template <typename Control>
    using get_function_type_t = typename get_function_type<Control>::type;

public:
    using functions_list = brigand::transform<controls_list, brigand::bind<get_function_type_t, brigand::_1>>;

    using any_control  = brigand::as_variant<controls_list>;
    using any_function = brigand::as_variant<functions_list>;

private:
    static constexpr inline std::size_t controls_count = 1;

public:
    virtual ~controls()
    {}
    controls(Args... args)
        : _control_map(add_controls(std::forward_as_tuple(args...)))
        , _keys(initialize_keys(std::forward_as_tuple(args...), std::make_index_sequence<controls_count>{}))
    {}

    std::error_code operator()(const keys::key_type key) override
    {
        if (!std::count(std::cbegin(_keys), std::cend(_keys), key))
        {
            return make_error_code(command_errc::unknown_control);
        }
        const auto ctrl = _control_map.at(key);
        std::visit([](auto & function) { std::invoke(function); }, ctrl);
        return {};
    }

private:
    const std::unordered_map<keys::key_type, any_function> _control_map;
    const std::array<keys::key_type, controls_count> _keys;

    std::unordered_map<keys::key_type, any_function> add_controls(std::tuple<Args...> args)
    {
        std::unordered_map<keys::key_type, any_function> cmds;
        std::apply(
            [&cmds](auto arg) {
                if constexpr (is_control_v<decltype(arg)>)
                {
                    cmds.insert({arg._key, arg._function});
                }
            },
            args);
        return cmds;
    }

    template <std::size_t... I>
    const std::array<keys::key_type, controls_count> initialize_keys(std::tuple<Args...> controls, std::index_sequence<I...>) const
    {
        return internal::make_array(std::get<I>(controls)._key...);
    }
};
} // namespace trm