#include <array>
#include <sstream>
#include <string>
#include <vector>

namespace term
{
namespace internal
{
template <typename D = void, typename... T>
constexpr auto make_array(T &&... t) -> std::array<std::conditional_t<std::is_void<D>::value, std::common_type_t<T...>, D>, sizeof...(T)>
{
    return {{std::forward<T>(t)...}};
}

std::vector<std::string> split(const std::string & s, char delim)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim))
    {
        elems.emplace_back(std::move(item));
    }
    return elems;
}

constexpr std::size_t length(const char * str)
{
    return (*str == 0) ? 0 : length(str + 1) + 1;
}

template <typename R, typename... Types>
constexpr std::size_t get_function_arg_count(R (*f)(Types...))
{
    return sizeof...(Types);
}

template <std::size_t I, typename R, typename... Types>
constexpr auto get_function_arg_type(R (*f)(Types...))
{
    auto t = std::tuple<Types...>{};
    return std::get<I>(t);
}
}
}