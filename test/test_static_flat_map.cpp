#include <catch2/catch.hpp>
#include <FlatMap/StaticFlatMap.hpp>

// TODO: add emplace()
// TODO: add static_assert on TriviallyCopyable
// TODO: add capacity() function (I guess same as max_size()) for consistency
// TODO: add erase(const key_type& key)
// TODO: maxMembers should be exposed constexpr:
//           probably both make max_size() constexpr, and
//           add static constexpr member

TEST_CASE("SFM insert", "[StaticFlatMap]")
{
    constexpr int kCount = 100;
    StaticFlatMap<int, int, 100> m;
    for (int i = 0; i < kCount; ++i) {
        auto it = m.Insert(std::make_pair(i, i + 1));
        REQUIRE(it != m.end());
        REQUIRE(it->first  == i);
        REQUIRE(it->second == i + 1);
    }
}

TEST_CASE("SFM lookup", "[StaticFlatMap]")
{
    constexpr int kCount = 100;
    StaticFlatMap<int, int, 100> m;
    for (int i = 0; i < kCount; ++i) {
        m.Insert(std::make_pair(i, i + 1));
    }

    SECTION("successful lookups") {
        for (int i = 0; i < kCount; ++i) {
            auto it = m.Find(i);
            REQUIRE(it != m.end());
            REQUIRE(it->first  == i);
            REQUIRE(it->second == i + 1);
        }
    }

    SECTION("unsuccessful lookups") {
        for (int i = kCount; i < 2*kCount; ++i) {
            auto it = m.Find(i);
            REQUIRE(it == m.end());
        }
    }
}

TEST_CASE("SFM erase", "[StaticFlatMap]")
{
    constexpr int kCount = 100;
    StaticFlatMap<int, int, 100> m;
    for (int i = 0; i < kCount; ++i) {
        m.Insert(std::make_pair(i, i + 1));
    }

    SECTION("successful erase by key") {
        REQUIRE(m.size() == static_cast<size_t>(kCount));

        for (int i = 0; i < kCount; i += 2) {
            auto it1 = m.Find(i);
            REQUIRE(it1 != m.end());

            auto it2 = m.Erase(i);
            auto it3 = m.Find(i+1);
            REQUIRE(it2 == it3);

            auto it4 = m.Find(i);
            REQUIRE(it4 == m.end());
        }

        REQUIRE(m.size() == static_cast<size_t>(kCount / 2));
    }

    // TODO: Erase(const KeyType& key) doesn't work like std::m::erase()
    //       which returns number of elements removed.
    // SECTION("unsuccessful erase by key") {
    //     REQUIRE(m.size() == 100u);
    //     for (int i = 100; i < 200; ++i) {
    //         auto it = m.Erase(i);
    //         REQUIRE(it == m.end());
    //     }
    //     REQUIRE(m.size() == 100u);
    // }
}

TEST_CASE("SFM lookup after erase and insert", "[StaticFlatMap]")
{
    constexpr int kCount = 100;
    StaticFlatMap<int, int, 100> m;
    for (int i = 0; i < kCount; ++i) {
        auto it = m.Insert(std::make_pair(i, i + 1));
        REQUIRE(it != m.end());
    }

    for (int i = 1; i < kCount; i += 2) {
        m.Erase(i);
    }

    for (int i = 1; i < kCount; i += 2) {
        auto it = m.Find(i);
        REQUIRE(it == m.end());
        m.Insert(std::make_pair(i, i + 2));
    }

    for (int i = 0; i < kCount; ++i) {
        auto it = m.Find(i);
        REQUIRE(it != m.end());
        REQUIRE(it->first  == i);
        REQUIRE(it->second == i + 1 + (i & 1));
    }
}
