#pragma once

#include <array>
#include <variant>

namespace trm
{
// clang-format off
    enum class ctrl_c
    {
        a = 1, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, up = 65, down, right, left
    };
    enum class alt
    {
        a = 97, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, up = 65, down, right, left
    };
    enum class alt_maj
    {
        a = 65, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, up = 65, down, right, left
    };
// clang-format on

struct keys
{
    using key_type = std::variant<char, ctrl_c, alt, alt_maj>;
    static const keys::key_type get(const std::array<char, 6> & buf);
    // {
    //     if (std::isprint(buf[0]))
    //     {
    //         return buf[0];
    //     }
    //     else if (buf[0] < 27)
    //     {
    //         return static_cast<ctrl_c>(buf[0]);
    //     }
    //     return 0;
    // }
};

} // namespace trm
