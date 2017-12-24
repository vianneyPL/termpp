#include <termpp/keys.hpp>
#include <algorithm>
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
            keys_map.insert(std::make_pair(arr, c));
        }
    }
}

static void initialize_ctrl(std::map<std::array<char, 6>, keys::keys::key_type> & keys_map)
{
    const auto start = 1;
    for (char c = start; c < start + 26; ++c)
    {
        std::array<char, 6> arr = {c};
        keys_map.insert(std::pair{arr, static_cast<keys::ctrl>(c)});
    }
    for (char c = 65; c < 69; ++c)
    {
        std::array<char, 6> arr = {27, 91, 49, 59, 53, c};
        keys_map.insert(std::make_pair(arr, static_cast<keys::ctrl>(c)));
    }
}

static void initialize_alt(std::map<std::array<char, 6>, keys::key_type> & keys_map)
{
    const auto start = 97;
    for (char c = start; c < start + 26; ++c)
    {
        std::array<char, 6> arr = {27, c};
        keys_map.insert(std::make_pair(arr, static_cast<keys::alt>(c)));
    }
    for (char c = 65; c < 69; ++c)
    {
        std::array<char, 6> arr = {27, 91, 49, 59, 51, c};
        keys_map.insert(std::make_pair(arr, static_cast<keys::alt>(c)));
    }
}

static void initialize_alt_maj(std::map<std::array<char, 6>, keys::key_type> & keys_map)
{
    const auto start = 65;
    for (char c = start; c < start + 26; ++c)
    {
        std::array<char, 6> arr = {27, c};
        keys_map.insert(std::make_pair(arr, static_cast<keys::alt_maj>(c)));
    }
    for (char c = 65; c < 69; ++c)
    {
        std::array<char, 6> arr = {27, 91, 49, 59, 52, c};
        keys_map.insert(std::make_pair(arr, static_cast<keys::alt_maj>(c)));
    }
}

static void initialize_normal(std::map<std::array<char, 6>, keys::key_type> & keys_map)
{
    {
        std::array<char, 6> arr = {27};
        keys_map.insert(std::make_pair(arr, keys::normal::escape));
    }
    {
        std::array<char, 6> arr = {127};
        keys_map.insert(std::make_pair(arr, keys::normal::del));
    }
    {
        std::array<char, 6> arr = {9};
        keys_map.insert_or_assign(arr, keys::normal::tab);
    }
    {
        std::array<char, 6> arr = {13};
        keys_map.insert_or_assign(arr, keys::normal::enter);
    }
    {
        std::array<char, 6> arr = {27, 91, 51, 126, 0, 0};
        keys_map.insert(std::make_pair(arr, keys::normal::suppr));
    }
    {
        for (char c = 65; c < 69; ++c)
        {
            std::array<char, 6> arr = {27, 91, c};
            keys_map.insert(std::make_pair(arr, static_cast<keys::normal>(c)));
        }
    }
}

static const std::map<std::array<char, 6>, keys::keys::key_type> initialize_keys()
{
    std::map<std::array<char, 6>, keys::key_type> keys_map;
    initialize_ctrl(keys_map);
    initialize_alt(keys_map);
    initialize_alt_maj(keys_map);
    initialize_normal(keys_map);
    initialize_printable(keys_map);
    return keys_map;
}

const keys::key_type keys::get(const std::array<char, 6> & buf)
{
    static const std::map<std::array<char, 6>, keys::key_type> _keys = initialize_keys();
    if (!_keys.count(buf)) return keys::none{};
    return _keys.at(buf);
}
} // namespace trm