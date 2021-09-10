include(GoogleTest)

add_executable(test-enc test_decoder.cpp)
target_link_libraries(test-enc PRIVATE gtest pthread nanocbor)

gtest_discover_tests(test-enc)
