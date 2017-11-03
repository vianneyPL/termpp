#pragma once

#include <string>

namespace term
{
template <typename T>
T parse(const std::string &arg);

template <>
int parse<int>(const std::string &arg)
{
    return std::stoi(arg);
}

template <>
long parse<long>(const std::string &arg)
{
    return std::stol(arg);
}

template <>
long long parse<long long>(const std::string &arg)
{
    return std::stoll(arg);
}

template <>
unsigned long parse<unsigned long>(const std::string &arg)
{
    return std::stoul(arg);
}

template <>
unsigned long long parse<unsigned long long>(const std::string &arg)
{
    return std::stoull(arg);
}

template <>
float parse<float>(const std::string &arg)
{
    return std::stof(arg);
}

template <>
double parse<double>(const std::string &arg)
{
    return std::stod(arg);
}

template <>
long double parse<long double>(const std::string &arg)
{
    return std::stold(arg);
}

template <>
std::string parse<std::string>(const std::string &arg)
{
    return arg;
}
}