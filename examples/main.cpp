#include <iostream>
#include <term.hpp>

int main()
{
    term::term t{};
    while (true)
    {
        auto ctrl = t.read_char();
        if (ctrl == term::ctrl::up)
        {
            std::cout << "exit\n\r";
            break;
        }
    }
}