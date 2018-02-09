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
    unknown_control,
    no_parser_found,
    parse_error
};

enum class input_errc
{
    bad_input = 100
};

std::error_code make_error_code(command_errc);
std::error_code make_error_code(input_errc);
} // namespace trm

namespace std
{
template <>
struct is_error_code_enum<trm::command_errc> : true_type
{};
template <>
struct is_error_code_enum<trm::input_errc> : true_type
{};
} // namespace std