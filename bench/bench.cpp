// Micro-benchmark harness: sequential fill, random fill, random read.
// Usage: tinykv_bench [num_keys] [value_bytes]
// Record results in docs/benchmarks.md next to the commit hash and machine details.

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

#include "tinykv/db.h"

namespace {

using Clock = std::chrono::steady_clock;

std::string make_key(std::uint64_t i) {
  char buf[24];
  std::snprintf(buf, sizeof buf, "key%016llu", static_cast<unsigned long long>(i));
  return buf;
}

template <typename F>
void run(const char* name, std::size_t ops, F&& body) {
  auto t0 = Clock::now();
  body();
  double secs = std::chrono::duration<double>(Clock::now() - t0).count();
  std::printf("%-12s %10zu ops  %8.3f s  %12.0f ops/s\n", name, ops, secs, static_cast<double>(ops) / secs);
}

}  // namespace

int main(int argc, char** argv) {
  std::size_t n = argc > 1 ? std::strtoull(argv[1], nullptr, 10) : 1'000'000;
  std::size_t value_bytes = argc > 2 ? std::strtoull(argv[2], nullptr, 10) : 100;
  std::string value(value_bytes, 'x');

  auto db = tinykv::DB::Open(tinykv::Options{});

  run("fillseq", n, [&] {
    for (std::size_t i = 0; i < n; ++i) db->Put(make_key(i), value);
  });

  std::mt19937_64 rng(42);
  std::vector<std::uint64_t> order(n);
  for (std::size_t i = 0; i < n; ++i) order[i] = rng() % n;

  run("fillrandom", n, [&] {
    for (auto k : order) db->Put(make_key(k), value);
  });

  std::size_t hits = 0;
  run("readrandom", n, [&] {
    for (auto k : order) hits += db->Get(make_key(k)).has_value();
  });
  std::printf("hits: %zu / %zu\n", hits, n);
  return 0;
}
