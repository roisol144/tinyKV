# tinykv

A small embedded key-value store built on a log-structured merge tree, written in C++20 as a learning project.

**Status:** milestone 0 (skeleton). `DB` is currently backed by `std::map`; each milestone below replaces a piece while the tests keep passing.

## Build and test

```bash
cmake -S . -B build
cmake --build build -j
ctest --test-dir build --output-on-failure
./build/bench/tinykv_bench 1000000 100
```

Sanitizers (use separate build directories):

```bash
cmake -S . -B build-asan -DTINYKV_SANITIZE=ON && cmake --build build-asan -j && ctest --test-dir build-asan
cmake -S . -B build-tsan -DTINYKV_TSAN=ON && cmake --build build-tsan -j && ctest --test-dir build-tsan
```

## Usage

```cpp
#include "tinykv/db.h"

auto db = tinykv::DB::Open({.path = "/tmp/mydb"});
db->Put("hello", "world");
auto v = db->Get("hello");   // std::optional<std::string>
db->Delete("hello");
```

## Milestones

| # | Milestone | Done |
|---|-----------|------|
| 0 | Skeleton, API, tests, benchmark harness | [x] |
| 1 | Skip-list memtable | [ ] |
| 2 | WAL with CRC and crash recovery | [ ] |
| 3 | SSTable writer/reader, bloom filter, flush | [ ] |
| 4 | Read path across memtable + SSTables, tombstones | [ ] |
| 5 | Compaction (background thread) + manifest | [ ] |
| 6 | Concurrency: snapshots, TSan clean | [ ] |
| 7 | Crash and fuzz testing | [ ] |
| 8 | Benchmarks vs RocksDB/LevelDB, optimization | [ ] |

See [docs/design.md](docs/design.md) for design questions to answer before each milestone and [docs/benchmarks.md](docs/benchmarks.md) for results.

## Layout

```
include/tinykv/   public headers
src/              implementation
tests/            unit tests (dependency-free harness)
bench/            benchmark driver
docs/             design notes and results
```
