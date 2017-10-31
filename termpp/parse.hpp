#pragma once

namespace term
{
template <typename T>
T parse(std::string arg);

template <>
int parse<int>(std::string arg)
{
    return std::atoi(arg.c_str());
}

template <>
std::string parse<std::string>(std::string arg)
{
    return arg;
}
}