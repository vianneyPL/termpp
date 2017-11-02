#include <commands.hpp>
#include <control.hpp>
#include <iostream>
#include <term.hpp>

constexpr int func(int a)
{
    return 2 * a;
}

std::string h(int a, std::string b)
{
    return std::string{"a: "} + std::to_string(a) + std::string{" - b: "} + b;
}

template <typename T>
void print(T && t)
{
    std::cout << __PRETTY_FUNCTION__ << '\n';
}

auto prepare()
{
    auto l = [](int a) { return std::string{"a: "} + std::to_string(a); };
    return term::commands(  //
        term::cmd("cmd", h) //
        ,
        term::cmd("asd", l) //
    );
}

int main()
{
    std::string cmd{"cmd 3 str"};
    std::string asd{"asd 3 str"};

    auto c = prepare();

    std::cout << c.call(cmd) << '\n';
    std::cout << c.call(asd) << '\n';

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