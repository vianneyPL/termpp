#include <termpp/keys.hpp>
#include <termpp/term.hpp>
#include <iostream>
#include <tuple>

namespace trm
{
#ifndef _WIN32
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

template <typename T>
void print()
{
    std::cout << __PRETTY_FUNCTION__;
}

static const auto get_key(int fd)
{
    std::array<char, 6> buf = {};

    int ret = read(fd, &buf, 6);
    if (ret < 0) return std::make_tuple(buf, keys::key_type{keys::none{}});
    return std::make_tuple(buf, keys::get(buf));
}

void term::run() noexcept
{
    reset_line();
    // reprint_line();
    while (!_is_exit)
    {
        const auto [buf, key] = get_key(_fd);
        // should be of the form:
        // getkey()
        // .then(process)
        // .fail(is_unknown_key)
        // .fail(/*fatal*/ exit)
        if (std::holds_alternative<keys::none>(key))
        {
            std::cerr << "Unknown key: ";
            auto delim = ' ';
            for (auto c : buf)
            {
                std::cerr << delim;
                std::cerr << std::to_string(static_cast<int>(c));
                delim = ',';
            }
            std::cerr << "\r\n\r";
        }
        else if (std::holds_alternative<char>(key))
        {
            char c = std::get<char>(key);
            _line.insert(c);
            reset_line();
        }
        else
        {
            auto err = std::invoke(*_ctrls, key);
        }
    }
}

bool term::is_supported()
{
    const std::string_view current = getenv("TERM");
    for (const auto & unsupported : unsupported_terms)
    {
        if (current == unsupported)
        {
            return false;
        }
    }
    return true;
}

void term::set_term()
{
    struct termios raw;

    if (!isatty(_fd) || !is_supported() || tcgetattr(_fd, &_orig) == -1)
    {
        std::cerr << "could not set terminal.\n";
        std::exit(-1);
    }

    raw = _orig; /* modify the original mode */
    /* input modes: no break, no CR to NL, no parity check, no strip char,
     * no start/stop output control. */
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    /* output modes - disable post processing */
    raw.c_oflag &= ~(OPOST);
    /* control modes - set 8 bit chars */
    raw.c_cflag |= (CS8);
    /* local modes - choing off, canonical off, no extended functions,
     * no signal chars (^Z,^C) */
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    /* control chars - set return condition: min number of bytes and timer.
     * We want read to return every single byte, without timeout. */
    raw.c_cc[VMIN]  = 1;
    raw.c_cc[VTIME] = 0; /* 1 byte, no timer */

    /* put terminal in raw mode after flushing */
    if (tcsetattr(_fd, TCSADRAIN, &raw) < 0)
    {
        std::cerr << "could not set terminal.\n";
        std::exit(-1);
    }
}
void term::unset_term()
{
    tcsetattr(_fd, TCSADRAIN, &_orig);
}

#else /* Windows platform, either MinGW or Visual Studio (MSVC) */
#include <windows.h>
#include <fcntl.h>

void term::set_term()
{
    DWORD n;
    INPUT_RECORD irec;

    current->outh = GetStdHandle(STD_OUTPUT_HANDLE);
    current->inh  = GetStdHandle(STD_INPUT_HANDLE);

    if (!PeekConsoleInput(current->inh, &irec, 1, &n))
    {
        std::exit(-1);
    }
    if (getWindowSize(current) != 0)
    {
        std::exit(-1);
    }
    if (GetConsoleMode(current->inh, &orig_consolemode))
    {
        SetConsoleMode(current->inh, ENABLE_PROCESSED_INPUT);
    }
}
void term::unset_term()
{
    SetConsoleMode(current->inh, orig_consolemode);
}

#endif

void term::reprint_line()
{
    auto i = _index;
    move_to_pos(std::size(_current));
    clear_line();
    move_to_pos(0);
    _index = i;
    write(1, _current.c_str(), std::size(_current));
    move_to_pos(_index);
    set_cursor_after();
}

void term::print_line()
{
    std::cout << "\r\n" << _line.content() << "\r\n";
    _line.reset();
    reset_line();
}
} // namespace trm
