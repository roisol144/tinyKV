#include "test_harness.h"
#include "tinykv/db.h"

using tinykv::DB;
using tinykv::Options;

TEST(put_then_get) {
  auto db = DB::Open(Options{});
  db->Put("k", "v");
  EXPECT_EQ(db->Get("k"), std::optional<std::string>("v"));
}

TEST(missing_key_returns_nullopt) {
  auto db = DB::Open(Options{});
  EXPECT_TRUE(!db->Get("nope").has_value());
}

TEST(overwrite_returns_latest) {
  auto db = DB::Open(Options{});
  db->Put("k", "1");
  db->Put("k", "2");
  EXPECT_EQ(db->Get("k"), std::optional<std::string>("2"));
}

TEST(delete_removes_key) {
  auto db = DB::Open(Options{});
  db->Put("k", "v");
  db->Delete("k");
  EXPECT_TRUE(!db->Get("k").has_value());
  db->Delete("never-existed");  // must not throw
}

TEST(empty_key_and_value) {
  auto db = DB::Open(Options{});
  db->Put("", "");
  EXPECT_EQ(db->Get(""), std::optional<std::string>(""));
}

// TODO(milestone 2): persists_across_reopen, torn_wal_tail_is_ignored
// TODO(milestone 3): flush_keeps_data_readable
// TODO(milestone 6): concurrent_readers_and_writer

int main() { return testing::run_all(); }
