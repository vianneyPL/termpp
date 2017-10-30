#include <control.hpp>
#include <iostream>
#include <term.hpp>

constexpr int func(int a)
{
    return 2 * a;
}

int main()
{
    std::string command{"cmd 3 str"};
    // constexpr auto l = [](int a) { return 2 * a; };
    constexpr term::control ctrl{std::string_view{"asd", 3}, func};
    // constexpr int a = ctrl.call(2);
    std::cout << ctrl.call(2) << '\n';

    // term::term t{};
    // t.run();
}