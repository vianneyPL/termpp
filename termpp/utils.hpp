#include <array>

namespace term
{
namespace internal
{
template <typename D = void, typename... T>
constexpr auto make_array(T &&... t) -> std::array<std::conditional_t<std::is_void<D>::value, std::common_type_t<T...>, D>, sizeof...(T)>
{
    return {{std::forward<T>(t)...}};
}
}
}