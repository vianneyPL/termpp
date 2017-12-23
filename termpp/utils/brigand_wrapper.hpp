#pragma once

#define BRIGAND_NO_BOOST_SUPPORT
#include <brigand.hpp>
#include <variant>

namespace brigand
{
template <typename... T>
using variant_wrapper = typename std::variant<T...>;
template <typename L>
using as_variant = wrap<L, variant_wrapper>;
} // namespace brigand
