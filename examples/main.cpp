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
    const auto commands = trm::internal::make_array("cmd 3 str", "asd 3", "bla 3");

    constexpr auto l = [](int a) { return std::to_string(a).c_str(); };

    auto c = trm::commands(trm::cmd("cmd", h), trm::cmd("asd", l));

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

    for (const auto & cmd : commands)
    {
        const auto tokens = trm::internal::split(cmd, ' ');
        auto [signature, error] = c.signature(tokens[0]);
        if (!error)
        {
            std::cout << ":: signature ::\n\t";
            for (const auto & arg : signature)
            {
                std::cout << " - " << arg;
            }
            std::cout << " - \n";
        }
        else
        {
            std::cerr << error << " - " << error.message() << '\n';
        }
    }
    // trm::term t{};
    // t.run();
}