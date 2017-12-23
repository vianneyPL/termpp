#include <termpp/keys.hpp>
#include <termpp/term.hpp>
#include <iostream>

namespace trm
{
#ifndef _WIN32
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

struct key_visitor
{
    key_visitor()
    {}
    template <typename Key>
    std::string operator()(const Key & k)
    {
        if constexpr (std::is_same_v<Key, keys::ctrl>)
        {
            return std::string{"ctrl: "} + std::to_string(static_cast<int>(k));
        }
        else if (std::is_same_v<Key, keys::alt>)
        {
            return std::string{"alt: "} + std::to_string(static_cast<int>(k));
        }
        else if (std::is_same_v<Key, keys::alt_maj>)
        {
            return std::string{"alt_maj: "} + std::to_string(static_cast<int>(k));
        }
        else if (std::is_same_v<Key, keys::normal>)
        {
            return std::string{"normal: "} + std::to_string(static_cast<int>(k));
        }
        else if (std::is_same_v<Key, char>)
        {
            return std::string{"char: "} + std::string{static_cast<char>(k)};
        }
        return std::string{"none"};
    }
};

void term::run() noexcept
{
    while (true)
    {
        std::array<char, 6> buf = {};

        int ret = read(_fd, &buf, 6);
        if (ret < 0) return;
        const auto & k = keys::get(buf);
        if (std::holds_alternative<keys::none>(k))
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
        else
        {
            std::string result = std::visit(key_visitor(), k);
            std::cout << result << "\r\n\r";
            continue;
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
} // namespace trm
