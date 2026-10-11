add_executable(tinykv_bench bench.cpp)
target_link_libraries(tinykv_bench PRIVATE tinykv tinykv_warnings)
