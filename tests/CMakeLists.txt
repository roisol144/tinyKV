add_executable(tinykv_tests test_db.cpp)
target_link_libraries(tinykv_tests PRIVATE tinykv tinykv_warnings)
add_test(NAME tinykv_tests COMMAND tinykv_tests)
