#include <catch2/catch.hpp>
#include <FlatMap/FlatMap.hpp>

TEST_CASE("FM insert", "[FlatMap]")
{
    FlatMap<int, int> m;
    REQUIRE(m.size() == 0u);
    REQUIRE(m.empty() == true);

    FlatMap<int, int>::iterator it1 = m.begin();
    FlatMap<int, int>::iterator it2 = m.end();
    REQUIRE(it1 == it2);
}
