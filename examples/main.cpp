#include <termpp/commands.hpp>
#include <termpp/control.hpp>
#include <iostream>
#include <termpp/term.hpp>

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


int main()
{
    std::array<std::string, 2> commands = {
        "cmd 3 str"
        , "asd 3 str"
    };

    constexpr auto l = [](int a) { return std::string{"a: "} + std::to_string(a); };

    constexpr auto c = term::commands(term::cmd("cmd", h) , term::cmd("asd", l));

    for (const auto &cmd : commands)
    {
        std::cout << ":: call ::\n";
        auto result = c.call(cmd);
        if (result != "")
        {
            std::cout << result << '\n';
        }
    }

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