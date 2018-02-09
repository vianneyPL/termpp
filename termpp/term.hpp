#pragma once

#include <termpp/controls/controls.hpp>
#include <termpp/line.hpp>
#include <termpp/utils/length.hpp>
#include <termpp/utils/make_array.hpp>
#include <termpp/utils/static_unique_pointer_cast.hpp>
#include <memory>
#include <string_view>

#ifdef _WIN32 /* Windows platform, either MinGW or Visual Studio (MSVC) */
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include <string>

namespace trm
{
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

    void set_controls(std::unique_ptr<controls_interface> ctrls)
    {
        _ctrls.swap(ctrls);
    }

    void reprint_line();
    void print_line();

    inline void move_to_pos(std::size_t x)
    {
        const std::string go_to_pos{"\r\x1b[" + std::to_string(x) + "C"};
        write(1, std::data(go_to_pos), std::size(go_to_pos));
        _index = x;
    }

    inline void set_cursor_after()
    {
        const std::string go_to_pos{"\r\x1b[" + std::to_string(_index + 1) + "C"};
        write(1, std::data(go_to_pos), std::size(go_to_pos));
    }

    inline void move_left()
    {
        auto index = _line.advance(-1);
        move_to_pos(index);
    }

    inline void move_right()
    {
        auto index = _line.advance(1);
        move_to_pos(index);
    }

    inline void del()
    {
        _line.del();
        reset_line();
    }

    inline void home()
    {
        constexpr const char * go_home = "\r";
        write(1, &go_home, 1);
        _index = 0;
    }

    inline void end_of_line()
    {
        const std::size_t line_size = std::size(_current);
        if (line_size)
        {
            move_to_pos(line_size);
        }
    }

    inline void exit()
    {
        _is_exit = true;
    }

    inline void new_line()
    {
        constexpr const char * newline = "\r\n";
        write(1, newline, 2);
    }

    inline void reset_line()
    {
        const auto & buffer = _line.buffer();
        constexpr std::string_view empty_line{"\x1b[2K\r", internal::length("\x1b[2K\r")};
        move_to_pos(std::size(buffer));
        write(1, std::data(empty_line), std::size(empty_line));
        write(1, std::data(buffer), std::size(buffer));
        move_to_pos(_line.index());
    }

private:
    line _line{};
    bool _is_exit{false};
    std::size_t _index{};
    std::string _current;
    std::unique_ptr<controls_interface> _ctrls;

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
} // namespace trm