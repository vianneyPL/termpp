#pragma once

#include <system_error>

namespace term
{
enum class cmd_errc
{
    empty_input = 10,
    not_enough_arguments,
    too_much_arguments,
    wrong_command,
    no_parser_found
};

std::error_code make_error_code(cmd_errc);
}

namespace std
{
template <>
struct is_error_code_enum<term::cmd_errc> : true_type
{
};
}