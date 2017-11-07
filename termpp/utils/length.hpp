#pragma once

namespace trm
{
namespace internal
{
constexpr std::size_t length(const char * str)
{
    return (*str == 0) ? 0 : length(str + 1) + 1;
}
}
}
