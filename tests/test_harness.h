#pragma once

// Tiny dependency-free test harness so the repo builds offline.
// Swap for GoogleTest or Catch2 later if you prefer.

#include <cstdio>
#include <cstdlib>
#include <functional>
#include <string>
#include <vector>

namespace testing {

struct Case {
  const char* name;
  std::function<void()> fn;
};

inline std::vector<Case>& registry() {
  static std::vector<Case> cases;
  return cases;
}

struct Registrar {
  Registrar(const char* name, std::function<void()> fn) { registry().push_back({name, std::move(fn)}); }
};

inline int& failures() {
  static int n = 0;
  return n;
}

inline int run_all() {
  for (auto& c : registry()) {
    int before = failures();
    c.fn();
    std::printf("[%s] %s\n", failures() == before ? " ok " : "FAIL", c.name);
  }
  std::printf("%zu tests, %d failed assertions\n", registry().size(), failures());
  return failures() == 0 ? 0 : 1;
}

}  // namespace testing

#define TEST(name)                                                  \
  static void test_##name();                                        \
  static testing::Registrar registrar_##name(#name, test_##name);   \
  static void test_##name()

#define EXPECT_TRUE(cond)                                                              \
  do {                                                                                 \
    if (!(cond)) {                                                                     \
      std::printf("  %s:%d: expected true: %s\n", __FILE__, __LINE__, #cond);          \
      ++testing::failures();                                                           \
    }                                                                                  \
  } while (0)

#define EXPECT_EQ(a, b) EXPECT_TRUE((a) == (b))
