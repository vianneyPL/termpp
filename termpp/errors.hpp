#pragma once

#include <system_error>

namespace trm
{
enum class command_errc
{
    empty_input = 10,
    not_enough_arguments,
    too_much_arguments,
    wrong_command,
    unknown_command,
    no_parser_found,
    parse_error
};

std::error_code make_error_code(command_errc);
} // namespace trm

namespace std
{
template <>
struct is_error_code_enum<trm::command_errc> : true_type
{};
} // namespace std