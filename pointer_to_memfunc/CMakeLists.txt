project(pointer_to_memfunc)
cmake_minimum_required(VERSION 2.9)

find_package(GTest REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17")

set(sources 
   main.cpp 
   pointer_to_memfunc.cpp)
add_executable(unit_tests ${sources})
target_link_libraries(unit_tests pthread)
target_link_libraries(unit_tests gtest)

enable_testing()
add_test(test_all unit_tests)

add_custom_target(run
    COMMAND unit_tests 
    DEPENDS unit_tests
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
