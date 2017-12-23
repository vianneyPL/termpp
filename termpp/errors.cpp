#include <termpp/errors.hpp>
#include <string>

namespace trm
{
namespace
{
struct cmd_err_category : std::error_category
{
    const char * name() const noexcept override;
    std::string message(int ev) const override;
};
const char * cmd_err_category::name() const noexcept
{
    return "command";
}
std::string cmd_err_category::message(int ev) const
{
    switch (static_cast<command_errc>(ev))
    {
    case command_errc::empty_input:
        return "empty input";
    case command_errc::not_enough_arguments:
        return "not enough arguments";
    case command_errc::too_much_arguments:
        return "too much arguments";
    case command_errc::wrong_command:
        return "wrong command";
    case command_errc::unknown_command:
        return "unknown command";
    case command_errc::no_parser_found:
        return "no parser found";
    case command_errc::parse_error:
        return "parse error";
    default:
        return "(unrecognized error)";
    }
}
const cmd_err_category concrete_cmd_err_category{};
} // namespace
std::error_code make_error_code(command_errc e)
{
    return {static_cast<int>(e), concrete_cmd_err_category};
}
} // namespace trm