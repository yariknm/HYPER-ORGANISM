cmake_minimum_required(VERSION 3.16)

find_package(GTest REQUIRED)

add_executable(hyper_organism_tests test_main.cpp)
target_link_libraries(hyper_organism_tests GTest::GTest GTest::Main hyper_organism)
target_include_directories(hyper_organism_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

include(GoogleTest)
gtest_discover_tests(hyper_organism_tests)