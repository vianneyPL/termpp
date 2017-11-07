#include "term.hpp"
#include <iostream>

namespace termpp
{
#ifndef _WIN32
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

void term::run() noexcept
{
    while (true)
    {
        char c;
        {
            int ret = read(_fd, &c, 1);
            if (ret < 0) return;
        }
        if (c == 27)
        {
            auto ctrl = read_ctrl();
            if (ctrl == ctrl::up)
            {
                std::cout << "exit\n\r";
                break;
            }
        }
        else
        {
            _current += c;
            const auto cl = clear_line();
            {
                int ret = write(_fd, cl.c_str(), cl.size());
                if (ret < 0) return;
            }
            {
                int ret = write(_fd, _current.c_str(), _current.size());
                if (ret < 0) return;
            }
        }
    }
}

ctrl term::read_ctrl() noexcept
{
    char c1;
    {
        int ret = read(_fd, &c1, 1);
        if (ret < 0) return ctrl::none;
    }
    char c2;
    {
        int ret = read(_fd, &c2, 1);
        if (ret < 0) return ctrl::none;
    }

    if (c1 == '[' || c1 == 'O')
    {
        switch (c2)
        {
        case 'A':
            return ctrl::up;
        case 'B':
            return ctrl::down;
        case 'C':
            return ctrl::right;
        case 'D':
            return ctrl::left;
        case 'F':
            return ctrl::end;
        case 'H':
            return ctrl::home;
        }
    }
    if (c1 == '[' && c2 >= '1' && c2 <= '8')
    {
        {
            int ret = read(_fd, &c1, 1);
            if (ret < 0) return ctrl::none;
        }
        /* extended escape */
        if (c1 == '~')
        {
            switch (c2)
            {
            case '2':
                return ctrl::insert;
            case '3':
                return ctrl::del;
            case '5':
                return ctrl::page_up;
            case '6':
                return ctrl::page_down;
            case '7':
                return ctrl::home;
            case '8':
                return ctrl::end;
            }
        }
        while (c1 != -1 && c1 != '~')
        {
            int ret = read(_fd, &c1, 1);
            if (ret < 0) return ctrl::none;
        }
    }
    return ctrl::none;
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
}
