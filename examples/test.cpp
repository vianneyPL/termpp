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

template <typename F, typename T, std::size_t... I>
void parser_impl(const std::vector<std::string> & tokens, F && f, T && t, std::index_sequence<I...>)
{
    f(parse<T>(tokens.at(I + 1))...);
}

template<std::size_t N, typename F, typename T, typename Indices = std::make_index_sequence<N>>
void parser_f(const std::vector<std::string> & tokens, F &&f)
{
    parser_impl(tokens, f, t, Indices{});
}


template<typename F>
void parser(const std::vector<std::string> & tokens, F &&f)
{
    constexpr auto arg_count = get_function_arg_count(f);
    parser_f<arg_count>(tokens, f);
}

template <std::size_t I, typename Tuple>
void apply_impl_at_index(std::vector<std::string> tokens, Tuple t)
{
    std::cout << "I:" << I << '\n';
    auto e = std::get<I>(t);
    std::cout << std::get<0>(e) << '\n';
    if (std::get<0>(e) == tokens[0])
    {
        parser(tokens, std::get<1>(e));
    }
}

template <typename Tuple, std::size_t...I>
void apply_impl(std::vector<std::string> tokens, Tuple t, std::index_sequence<I...>)
{
    (apply_impl_at_index<I>(tokens, t), ...);
}

template <typename Tuple>
void apply(std::vector<std::string> tokens, Tuple t)
{
    apply_impl(tokens, t, std::make_index_sequence<std::tuple_size<Tuple>{}>{});
}


void caller(std::string to_parse)
{
    const auto tokens = term::internal::split(to_parse, ' ');
    // std::tuple<std::tuple<std::string_view, void(int, std::string)>> t;
    constexpr auto t = std::make_tuple(std::make_tuple("cmd", h));
    apply(tokens, t);
}


int main()
{
    std::string command{"cmd 3 str"};

    caller(command);

    // constexpr auto l = [](int a) { return 2 * a; };
    // constexpr term::control ctrl{std::string_view{"asd", 3}, func};
    // constexpr int a = ctrl.call(2);
    // std::cout << ctrl.call(2) << '\n';

    // term::term t{};
    // t.run();
}