#include "tinykv/db.h"

namespace tinykv {

DB::DB(const Options& options) : options_(options) {}
DB::~DB() = default;

std::unique_ptr<DB> DB::Open(const Options& options) {
  return std::unique_ptr<DB>(new DB(options));
}

void DB::Put(std::string_view key, std::string_view value) {
  data_.insert_or_assign(std::string(key), std::string(value));
}

std::optional<std::string> DB::Get(std::string_view key) const {
  auto it = data_.find(key);
  if (it == data_.end()) return std::nullopt;
  return it->second;
}

void DB::Delete(std::string_view key) {
  auto it = data_.find(key);
  if (it != data_.end()) data_.erase(it);
}

}  // namespace tinykv
