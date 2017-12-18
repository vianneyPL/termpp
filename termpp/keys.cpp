#include "keys.hpp"
#include <iostream>
#include <limits>
#include <map>

namespace trm
{

static void initialize_printable(std::map<std::array<char, 6>, keys::key_type> & keys_map)
{
    for (char c = 0; c < std::numeric_limits<char>::max(); ++c)
    {
        if (std::isprint(c))
        {
            std::array<char, 6> arr = {c};
            keys_map.insert(std::pair{arr, c});
        }
    }
}

static void initialize_ctrl(std::map<std::array<char, 6>, keys::keys::key_type> & keys_map)
{
    const auto start = 1;
    for (char c = start; c < start + 26; ++c)
    {
        std::array<char, 6> arr = {c};
        keys_map.insert(std::pair{arr, static_cast<ctrl_c>(c)});
    }
    for (char c = 65; c < 69; ++c)
    {
        std::array<char, 6> arr = {27, 91, 49, 59, 53, c};
        keys_map.insert(std::pair{arr, static_cast<ctrl_c>(c)});
    }
}

static void initialize_alt(std::map<std::array<char, 6>, keys::key_type> & keys_map)
{
    const auto start = 97;
    for (char c = start; c < start + 26; ++c)
    {
        std::array<char, 6> arr = {27, c};
        keys_map.insert(std::pair{arr, static_cast<alt>(c)});
    }
    for (char c = 65; c < 69; ++c)
    {
        std::array<char, 6> arr = {27, 91, 49, 59, 51, c};
        keys_map.insert(std::pair{arr, static_cast<alt>(c)});
    }
}

static void initialize_alt_maj(std::map<std::array<char, 6>, keys::key_type> & keys_map)
{
    const auto start = 65;
    for (char c = start; c < start + 26; ++c)
    {
        std::array<char, 6> arr = {27, c};
        keys_map.insert(std::pair{arr, static_cast<alt_maj>(c)});
    }
    for (char c = 65; c < 69; ++c)
    {
        std::array<char, 6> arr = {27, 91, 49, 59, 52, c};
        keys_map.insert(std::pair{arr, static_cast<alt_maj>(c)});
    }
}

static const std::map<std::array<char, 6>, keys::keys::key_type> initialize_keys()
{
    std::map<std::array<char, 6>, keys::key_type> keys_map;
    initialize_ctrl(keys_map);
    initialize_printable(keys_map);
    initialize_alt(keys_map);
    initialize_alt_maj(keys_map);
    return keys_map;
}

const keys::key_type keys::get(const std::array<char, 6> & buf)
{
    static const std::map<std::array<char, 6>, keys::key_type> _keys = initialize_keys();
    return _keys.at(buf);
}
} // namespace trm