#include <termpp/commands.hpp>
#include <termpp/term.hpp>
#include <iostream>

std::string h(long a, std::string b)
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
    std::array<std::string, 2> commands = {"cmd 3 str", "asd 3 str"};

    constexpr auto l = [](int a) { return std::string{"a: "} + std::to_string(a); };

    auto c = trm::commands(trm::cmd("cmd", h));

    for (const auto & cmd : commands)
    {
        std::cout << ":: call ::\n";
        auto[result, error] = c.call(cmd);
        if (!error)
        {
            std::cout << result << '\n';
        }
        else
        {
            std::cerr << error << " - " << error.message() << '\n';
        }
    }

    // print(c);

    // auto c = trm::commands(
    //     trm::cmd("cmd", h)
    //     // , trm::cmd("cmd2", [](int a) { std::cout  << "a: " << a << '\n'; })
    // );
    // print(c);

    // auto c = trm::make_commands();

    // caller(command);

    // constexpr auto l = [](int a) { return 2 * a; };
    // constexpr trm::control ctrl{std::string_view{"asd", 3}, func};
    // constexpr int a = ctrl.call(2);
    // std::cout << ctrl.call(2) << '\n';

    // trm::term t{};
    // t.run();
}