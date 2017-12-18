#include <termpp/cmd.hpp>
#include <catch.hpp>
#include <string>
#include <vector>

std::string func(int a, char * s)
{
    return {};
}

TEST_CASE("command", "[command]")
{

    SECTION("name")
    {
        SECTION("should not create with zero length name")
        {
            REQUIRE_THROWS(trm::cmd("", func));
        }

        SECTION("should not create with zero length")
        {
            auto c = trm::cmd("cmd", func);
            REQUIRE(c.name() == "cmd");
        }
    }

    SECTION("signature")
    {
        auto c = trm::cmd("cmd", func);
        REQUIRE(c.signature() == std::vector<std::string>({"int", "char *"}));
    }
}