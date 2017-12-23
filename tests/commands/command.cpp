#include <termpp/commands/command.hpp>
#include <catch.hpp>
#include <string>
#include <vector>

std::string func(int a, char * s)
{
    return {};
}
std::string func_with_ref(int & a, char * s)
{
    return {};
}

TEST_CASE("command", "[command]")
{
    auto c          = trm::command("cmd", func);
    auto c_with_ref = trm::command("cmd", func_with_ref);

    SECTION("name")
    {
        SECTION("should not create with zero length name")
        {
            REQUIRE_THROWS(trm::command("", func));
        }

        SECTION("should create with normal name")
        {
            REQUIRE(c.name() == "cmd");
        }
    }

    SECTION("signature")
    {
        REQUIRE(c.signature() == std::vector<std::string>({"int", "char *"}));
    }

    // TODO(vianney): Need to make command::operator() without template Args... for those tests to work
    // SECTION("function call")
    // {
    //     SECTION("should be able to call func without ref")
    //     {
    //         REQUIRE(boost::callable_traits::is_invocable_v<decltype(c), int, char *>);
    //     }
    //     SECTION("should be able to call func_with_ref with ref")
    //     {
    //         REQUIRE(boost::callable_traits::is_invocable_v<decltype(c_with_ref), int &, char *>);
    //     }
    //     SECTION("should not be able to call func_with_ref without ref")
    //     {
    //         REQUIRE(!boost::callable_traits::is_invocable_v<decltype(c_with_ref), int, char *>);
    //     }
    // }
}