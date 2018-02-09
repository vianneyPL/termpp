#include <termpp/term.hpp>
#include <iostream>

int main()
{
    trm::term t{};
    const auto exit_1 = trm::control{trm::keys::ctrl::c, [&t] { t.exit(); }};
    const auto exit_2 = trm::control{trm::keys::ctrl::d, [&t] { t.exit(); }};
    const auto del    = trm::control{trm::keys::normal::del, [&t] { t.del(); }};
    const auto left   = trm::control{trm::keys::normal::left, [&t] { t.move_left(); }};
    const auto right  = trm::control{trm::keys::normal::right, [&t] { t.move_right(); }};
    const auto enter  = trm::control{trm::keys::normal::enter, [&t] { t.print_line(); }};
    // print<decltype(ctrls)>();
    t.set_controls(trm::make_controls(enter, exit_1, exit_2, left, right, del));
    t.run();
    return 1;
}