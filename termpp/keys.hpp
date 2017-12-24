#pragma once

#include <array>
#include <variant>

namespace trm
{

struct keys
{
    enum class none;
    // clang-format off
    enum class ctrl
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
    enum class normal
    {
        tab    = 9,
        enter  = 13,
        escape = 27,
        up     = 65,
        down,
        right,
        left,
        endofline = 70,
        home      = 72,
        del       = 127,
        suppr
    };

    using key_type = std::variant<char, ctrl, alt, alt_maj, none, normal>;
    static const keys::key_type get(const std::array<char, 6> & buf);
};

} // namespace trm
