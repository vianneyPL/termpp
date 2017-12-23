#pragma once

#include "args.hpp"
#include <boost/callable_traits.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace trm
{
template <typename Cmd>
auto convert(const Cmd & cmd, const std::vector<std::string> & tokens)
{
    Cmd::args_type args;
    std::error_code error;
    std::size_t index = 1;

    const auto parse_arg = [&error](auto & arg) {
        using arg_type = std::remove_reference_t<decltype(arg)>;
        if (!error)
        {
            std::tie(arg, error) = arg_impl<arg_type>::parse(tokens.at(index));
            if (error)
            {
                std::cerr << "Error while parsing `" << cmd.name() << "` expected type `" << arg_impl<arg_type>::signature() << "` got `"
                          << tokens.at(index) << "`\n";
            }
            ++index;
        }
    };
    std::apply(parse_arg, args);
    return std::tuple{args, error};
}
} // namespace trm