#include <iostream>
#include <forward_list>
#include <queue>
#include <string>
#include <vector>
#include <memory>
#include <benchmark/benchmark.h>

#include <algorithm>
#include <ranges>

//
// This example demonstrates how to use a priority queue for
// partially sorting a list which only provides forward iterators.
//
namespace {

class Document {
public:
  Document(const std::string& title)
  : title_{title}
  {}
  auto title() const {
    return title_;
  }
private:
  std::string title_;
};

struct Hit {
  float rank_{};
  std::shared_ptr<Document> document_;
};

template<typename It>
auto sort_hits(It begin, It end, size_t m) -> std::vector<Hit> {
  auto cmp = [](const Hit& a, const Hit& b) {
    return a.rank_ > b.rank_; // Note, we are using greater than
  };
  auto queue = std::priority_queue<Hit, std::vector<Hit>, decltype(cmp)>{cmp};

  for (auto it = begin; it != end; ++it) {
    if (queue.size() < m) {
      queue.push(*it);
    }
    else if (it->rank_ > queue.top().rank_) {
      queue.pop();
      queue.push(*it);
    }
  }

  auto result = std::vector<Hit>{};
  while (!queue.empty()) {
    result.push_back(queue.top());
    queue.pop();
  }
  std::reverse(result.begin(), result.end());
  return result;
}

// Utility to create some test data
auto generate_random_hit(const std::string& title) {
  auto hit = Hit{};
  hit.document_ = std::make_shared<Document>(title);
  hit.rank_ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  return hit;
}

} // namespace


static void priority_queue_partial_search(benchmark::State& state) {
  auto n = static_cast<int>(state.range(0));
  auto hits = std::forward_list<Hit>{};
  for (auto i = 0; i <= n; ++i) {
    hits.emplace_front(generate_random_hit(std::to_string(i)));
  }
  for (auto _ : state) {
    benchmark::DoNotOptimize(sort_hits(hits.begin(), hits.end(), 10));
  }
  benchmark::ClobberMemory(); // Ensure the sort is not optimized away
}

static void std_ranges_sort(benchmark::State& state) {
  auto n = static_cast<int>(state.range(0));
  auto hits = std::vector<Hit>{};
  for (auto i = 0; i <= n; ++i) {
    hits.emplace_back(generate_random_hit(std::to_string(i)));
  }
  for (auto _ : state) {
    benchmark::DoNotOptimize(std::ranges::sort(hits, [](const Hit& a, const Hit& b) {
      return a.rank_ > b.rank_;
    }));
  }
  benchmark::ClobberMemory(); // Ensure the sort is not optimized away
}

static void std_partial_sort(benchmark::State& state) {
  auto n = static_cast<int>(state.range(0));
  auto hits = std::vector<Hit>{};
  for (auto i = 0; i <= n; ++i) {
    hits.emplace_back(generate_random_hit(std::to_string(i)));
  }
  for (auto _ : state) {
    std::partial_sort(hits.begin(), std::next(hits.begin(), n), hits.end(),
      [](const Hit& a, const Hit& b) {
        return a.rank_ > b.rank_;
    });
  }
  benchmark::ClobberMemory(); // Ensure the sort is not optimized away
}

constexpr size_t START = 262'144;
constexpr size_t END = START * 8;
BENCHMARK(priority_queue_partial_search)->RangeMultiplier(2)->Range(START, END);
BENCHMARK(std_ranges_sort)->RangeMultiplier(2)->Range(START, END);
BENCHMARK(std_partial_sort)->RangeMultiplier(2)->Range(START, END);

BENCHMARK_MAIN();

// Seems like partial sorting is faster than full sorting only when the number of elements is 4M+.