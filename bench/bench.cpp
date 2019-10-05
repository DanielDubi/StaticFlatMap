#include <benchmark/benchmark.h>
#include <FlatMap/StaticFlatMap.hpp>
#include <map>
#include <unordered_set>
#include <vector>
#include <utility>
#include <random>
#include <cassert>
#include <climits>


#define SUCCESSFUL_LOOKUP_BENCH 1
#define COPY_MAP_BENCH          1


// The Google.Benchmark macros don't play nicely with templated types
// because of the commas. Have to love macros...
using IntIntStlMap = std::map<int, int>;
using IntIntStaticFlatMap32  = StaticFlatMap<int, int, 32>;
using IntIntStaticFlatMap64  = StaticFlatMap<int, int, 64>;
using IntIntStaticFlatMap128 = StaticFlatMap<int, int, 128>;
using IntIntStaticFlatMap256 = StaticFlatMap<int, int, 256>;

// -----------------------------------------------------------------------------
// Data Generators
//

auto getIntMapData(size_t n, int min = INT_MIN, int max = INT_MAX)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);

    std::unordered_set<int> vs;
    while (vs.size() < 2*n) {
        vs.insert(dist(gen));
    }

    std::vector<std::pair<int, int>> present;
    std::vector<int>                 missing;
    for (auto v : vs) {
        if (present.size() < n) {
            present.emplace_back(v, dist(gen));
        } else {
            missing.emplace_back(v);
        }
    }
    assert(present.size() == n);
    assert(missing.size() == n);

    std::shuffle(present.begin(), present.end(), gen);
    std::shuffle(missing.begin(), missing.end(), gen);
    std::shuffle(present.begin(), present.end(), gen);
    std::shuffle(missing.begin(), missing.end(), gen);

    return std::make_pair(present, missing);
}

template <class KV>
auto getRandomData(
        const std::vector<KV>& vs,
        const std::vector<typename KV::first_type>& ms,
        int n,            // number of elements
        double successPct // percent of elements to pull from `vs`
    )
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist1(0, vs.size() - 1);
    std::uniform_int_distribution<int> dist2(0, ms.size() - 1);
    std::uniform_int_distribution<int> prob(0, 100);

    using Key = typename KV::first_type;
    std::vector<Key> rs;
    rs.reserve(n);

    successPct = std::max(std::min(successPct, 100.0), 0.0);
    int present = n * successPct;
    int missing = n - present;

    std::generate_n(
        std::back_inserter(rs),
        present,
        [&]() -> Key { return vs[dist1(gen)].first; }
    );

    std::generate_n(
        std::back_inserter(rs),
        missing,
        [&]() -> Key { return ms[dist2(gen)]; }
    );

    // for good measure...
    std::shuffle(rs.begin(), rs.end(), gen);
    std::shuffle(rs.begin(), rs.end(), gen);
    std::shuffle(rs.begin(), rs.end(), gen);

    return rs;
}

// -----------------------------------------------------------------------------
// Successful Lookup Benchmarks
//
#if SUCCESSFUL_LOOKUP_BENCH

#define SUCCESSFUL_LOOKUP_ARGS \
    ->Args({1<<10, 10})   \
    ->Args({2<<10, 10})   \
    ->Args({1<<10, 20})   \
    ->Args({2<<10, 20})   \
    ->Args({1<<10, 32})   \
    ->Args({2<<10, 32})   \

template <class Map>
static void BM_SuccessfulLookups(benchmark::State& state) {
    int count = 0;
    for (auto _ : state) {
        state.PauseTiming();
        Map m;
        auto vals = getIntMapData(state.range(1));
        for (auto&& v : vals.first) {
            m.insert(v);
        }
        auto data = getRandomData(vals.first, vals.second, state.range(0), 1.0);
        state.ResumeTiming();

        for (auto key : data) {
            auto it = m.find(key);
            benchmark::DoNotOptimize(count += it == m.end());
        }
    }

    // just to be safe, use `count` so compiler doesn't optimize away
    if (count != 0) {
        throw std::runtime_error("");
    }
}
BENCHMARK_TEMPLATE(BM_SuccessfulLookups, IntIntStlMap          ) SUCCESSFUL_LOOKUP_ARGS;
BENCHMARK_TEMPLATE(BM_SuccessfulLookups, IntIntStaticFlatMap32 ) SUCCESSFUL_LOOKUP_ARGS;
BENCHMARK_TEMPLATE(BM_SuccessfulLookups, IntIntStaticFlatMap64 ) SUCCESSFUL_LOOKUP_ARGS;
BENCHMARK_TEMPLATE(BM_SuccessfulLookups, IntIntStaticFlatMap128) SUCCESSFUL_LOOKUP_ARGS;
BENCHMARK_TEMPLATE(BM_SuccessfulLookups, IntIntStaticFlatMap256) SUCCESSFUL_LOOKUP_ARGS;

#endif

// -----------------------------------------------------------------------------
// Copy Map Benchmark
//
#if COPY_MAP_BENCH

#define COPY_MAP_ARGS \
    ->Arg(10)         \
    ->Arg(20)         \
    ->Arg(30)         \

template <class Map>
static void BM_CopyMap(benchmark::State& state) {
    size_t count = 0;
    for (auto _ : state) {
        state.PauseTiming();
        Map m;
        for (auto&& v : getIntMapData(state.range(0)).first) {
            m.insert(v);
        }
        state.ResumeTiming();

        Map m2 = m;
        benchmark::DoNotOptimize(count += m2.size());
    }

    // just to be safe, use `count` so compiler doesn't optimize away
    if (count == 0u) {
        throw std::runtime_error("");
    }
}
BENCHMARK_TEMPLATE(BM_CopyMap, IntIntStlMap          ) COPY_MAP_ARGS;
BENCHMARK_TEMPLATE(BM_CopyMap, IntIntStaticFlatMap32 ) COPY_MAP_ARGS;
BENCHMARK_TEMPLATE(BM_CopyMap, IntIntStaticFlatMap64 ) COPY_MAP_ARGS;
BENCHMARK_TEMPLATE(BM_CopyMap, IntIntStaticFlatMap128) COPY_MAP_ARGS;
BENCHMARK_TEMPLATE(BM_CopyMap, IntIntStaticFlatMap256) COPY_MAP_ARGS;

#endif

BENCHMARK_MAIN();
