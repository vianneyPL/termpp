#include <control.hpp>
#include <iostream>
#include <string>
#include <tuple>
#include <term.hpp>

constexpr int func(int a)
{
    return 2 * a;
}

void h(int a, std::string b)
{
    std::cout << "a: " << a << " - b: " << b << '\n';
}

template <typename T>
T parse(std::string arg);

template <>
int parse<int>(std::string arg)
{
    return std::atoi(arg.c_str());
}

template <>
std::string parse<std::string>(std::string arg)
{
    return arg;
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

template <typename F, std::size_t... I>
void parser_impl(const std::vector<std::string> & tokens, F && f, std::index_sequence<I...>)
{
    f(parse<decltype(get_function_arg_type<I>(f))>(tokens.at(I + 1))...);
}

template<std::size_t N, typename F, typename Indices = std::make_index_sequence<N>>
void parser(const std::vector<std::string> & tokens, F &&f)
{
    parser_impl(tokens, f, Indices{});
}

template <typename F>
void caller(F && f, std::string to_parse)
{
    const auto tokens = term::internal::split(to_parse, ' ');
    parser<get_function_arg_count(f)>(tokens, f);
}


int main()
{
    std::string command{"cmd 3 str"};

    caller(h, command);

    // constexpr auto l = [](int a) { return 2 * a; };
    // constexpr term::control ctrl{std::string_view{"asd", 3}, func};
    // constexpr int a = ctrl.call(2);
    // std::cout << ctrl.call(2) << '\n';

    // term::term t{};
    // t.run();
}