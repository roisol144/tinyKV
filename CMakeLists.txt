cmake_minimum_required(VERSION 3.20)
project(tinykv VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

option(TINYKV_SANITIZE "Build with address+undefined sanitizers" OFF)
option(TINYKV_TSAN "Build with thread sanitizer (exclusive with TINYKV_SANITIZE)" OFF)
option(TINYKV_BUILD_TESTS "Build tests" ON)
option(TINYKV_BUILD_BENCH "Build benchmarks" ON)

add_library(tinykv_warnings INTERFACE)
target_compile_options(tinykv_warnings INTERFACE -Wall -Wextra -Wpedantic -Wshadow -Wconversion)

if(TINYKV_SANITIZE AND TINYKV_TSAN)
  message(FATAL_ERROR "TINYKV_SANITIZE and TINYKV_TSAN are mutually exclusive")
endif()
if(TINYKV_SANITIZE)
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
elseif(TINYKV_TSAN)
  add_compile_options(-fsanitize=thread -fno-omit-frame-pointer)
  add_link_options(-fsanitize=thread)
endif()

add_library(tinykv src/db.cpp)
target_include_directories(tinykv PUBLIC include)
target_link_libraries(tinykv PRIVATE tinykv_warnings)

if(TINYKV_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(TINYKV_BUILD_BENCH)
  add_subdirectory(bench)
endif()
