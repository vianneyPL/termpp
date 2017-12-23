#include <termpp/term.hpp>
#include <iostream>

template <typename T>
void print(T && t)
{
    std::cout << __PRETTY_FUNCTION__ << '\n';
}

int main()
{
    trm::term t{};
    t.run();
    return 1;
}