#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace term
{
namespace internal
{
std::vector<std::string> split(const std::string & s, char delim)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim))
    {
        elems.emplace_back(std::move(item));
    }
    return elems;
}
}
}
