#include <control.hpp>
#include <iostream>
#include <term.hpp>
#include <commands.hpp>

constexpr int func(int a)
{
    return 2 * a;
}

void h(int a, std::string b)
{
    std::cout << "a: " << a << " - b: " << b << '\n';
}

template <typename T>
void print(T &&t)
{
    std::cout << __PRETTY_FUNCTION__ << '\n';
}

int main()
{
    std::string cmd{"cmd 3 str"};
    std::string asd{"asd 3"};
    auto c = term::commands(
        term::cmd("cmd", h)
        , term::cmd("asd", [](int a) { std::cout << "a: " << a << '\n'; })
    );

    c.call(cmd);
    c.call(asd);

    // print(c);

    
    // auto c = term::commands(
    //     term::cmd("cmd", h)
    //     // , term::cmd("cmd2", [](int a) { std::cout  << "a: " << a << '\n'; })
    // );
    // print(c);

    // auto c = term::make_commands();

    // caller(command);

    // constexpr auto l = [](int a) { return 2 * a; };
    // constexpr term::control ctrl{std::string_view{"asd", 3}, func};
    // constexpr int a = ctrl.call(2);
    // std::cout << ctrl.call(2) << '\n';

    // term::term t{};
    // t.run();
}