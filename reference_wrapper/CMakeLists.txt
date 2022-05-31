project(reference_wrapper)
cmake_minimum_required(VERSION 2.9)

find_package(GTest REQUIRED)
add_definitions(-std=c++11)

set(sources 
   main.cpp 
   reference_wrapper.cpp)
add_executable(unit_tests ${sources})
target_link_libraries(unit_tests gtest)
target_link_libraries(unit_tests pthread)

enable_testing()
add_test(test_all unit_tests)

add_custom_target(run
    COMMAND unit_tests 
    DEPENDS unit_tests
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
