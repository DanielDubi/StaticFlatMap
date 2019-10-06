#include <catch2/catch.hpp>
#include <FlatMap/FlatMap.hpp>

TEST_CASE("FM empty", "[FlatMap]")
{
    FlatMap<int, int> m;
    REQUIRE(m.size() == 0u);
    REQUIRE(m.empty() == true);

    FlatMap<int, int>::iterator it1 = m.begin();
    FlatMap<int, int>::iterator it2 = m.end();
    REQUIRE(it1 == it2);
}

TEST_CASE("FM lower_bound", "[FlatMap]")
{
    FlatMap<int, int> m;
    FlatMap<int, int>::iterator it = m.lower_bound(3);
    REQUIRE(it == m.end());

    FlatMap<int, int> m2;
    m.swap(m2);

    using std::swap;
    swap(m, m2);
}
