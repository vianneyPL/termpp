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
    auto ctrl = trm::control{trm::keys::normal::enter, [&t] { t.print_line(); }};
    t.set_controls(std::move(std::unique_ptr<trm::controls_interface>(static_cast<trm::controls_interface *>(new trm::controls{ctrl}))));
    t.run();
    return 1;
}