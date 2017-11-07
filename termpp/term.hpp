#pragma once
#include <termpp/utils/make_array.hpp>

#ifdef _WIN32 /* Windows platform, either MinGW or Visual Studio (MSVC) */
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include <string>

namespace trm
{
enum class ctrl
{
    none      = 0,
    up        = -20,
    down      = -21,
    left      = -22,
    right     = -23,
    del       = -24,
    home      = -25,
    end       = -26,
    insert    = -27,
    page_up   = -28,
    page_down = -29
};

class term
{

public:
    term()
    {
        set_term();
    }
    ~term()
    {
        unset_term();
    }

    void run() noexcept;

private:
    ctrl read_ctrl() noexcept;

private:
    std::string _current;

#ifndef _WIN32
    int _fd = STDIN_FILENO;
    struct termios _orig;
    bool is_supported();
    static inline constexpr auto unsupported_terms = internal::make_array("dumb", "cons25", nullptr);
#else
#endif

    void set_term();
    void unset_term();
};

inline std::string repeat(unsigned n, std::string_view s)
{
    std::string result = "";
    for (unsigned i = 0; i < n; ++i)
    {
        result.append(s);
    }
    return result;
}

inline std::string clear_before_cursor()
{
    return "\e[0K";
}

inline std::string clear_after_cursor()
{
    return "\e[1K";
}

inline std::string clear_line()
{
    return "\e[2K\r";
}

inline std::string move_up(unsigned n = 1)
{
    return "\e[" + std::to_string(n) + "A\r";
}

inline std::string clear_lines(unsigned n = 1)
{
    return "\e[0m" + clear_before_cursor() + ((n) ? repeat(n, clear_line() + move_up()) : std::string(""));
}
}