#pragma once

#include <string_view>

namespace termpp
{
template <typename F>
class control
{
public:
    constexpr control(std::string_view name, F && f)
        : _name{name}
        , _f{f}
    {}

    template <typename... Args>
    constexpr auto call(Args... args) const
    {
        return _f(args...);
    }

private:
    std::string_view _name;
    F & _f;
};
}