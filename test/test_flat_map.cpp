#include <catch2/catch.hpp>
#include <FlatMap/FlatMap.hpp>

TEST_CASE("FM insert", "[FlatMap]")
{
    // FlatMap<int, int> m;
    FlatMap<int, int>::iterator it1;
    FlatMap<int, int>::iterator it2;
    REQUIRE(it1 == it2);
    REQUIRE(1 == 0);
}
