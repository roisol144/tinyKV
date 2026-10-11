#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace tinykv {

struct Options {
  // Directory holding the WAL and SSTables. Unused until milestone 2.
  std::string path;
  // Memtable size that triggers a flush (milestone 3).
  std::size_t memtable_bytes = 4 * 1024 * 1024;
  // fsync after every write? Trades throughput for durability (milestone 2).
  bool sync_writes = false;
};

// Public API. The implementation is replaced step by step following docs/design.md;
// tests and benchmarks only depend on this interface.
class DB {
 public:
  static std::unique_ptr<DB> Open(const Options& options);
  ~DB();

  DB(const DB&) = delete;
  DB& operator=(const DB&) = delete;

  void Put(std::string_view key, std::string_view value);
  std::optional<std::string> Get(std::string_view key) const;
  void Delete(std::string_view key);

 private:
  explicit DB(const Options& options);

  Options options_;
  // Milestone 0 placeholder: replaced by a skip-list memtable.
  std::map<std::string, std::string, std::less<>> data_;
};

}  // namespace tinykv
