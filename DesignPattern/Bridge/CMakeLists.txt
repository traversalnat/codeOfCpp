project(Bridge)
cmake_minimum_required(VERSION 2.9)

include_directories($ENV{GMOCK_HOME}/include $ENV{GMOCK_HOME}/gtest/include)
link_directories($ENV{GMOCK_HOME}/mybuild $ENV{GMOCK_HOME}/gtest/mybuild)
add_definitions(-std=c++0x)
set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -Wall")

set (headers
	Abstraction.h
	AbstractionImpl.h)

set(sources 
	main.cpp 
	Bridge.cpp
	Abstraction.cpp
	AbstractionImpl.cpp
	Bridge.cpp
	main.cpp)
add_executable(unit_tests ${headers} ${sources})
target_link_libraries(unit_tests pthread)
target_link_libraries(unit_tests gmock)
target_link_libraries(unit_tests gtest)

enable_testing()
add_test(test_all unit_tests)

add_custom_target(run
	COMMAND unit_tests 
    DEPENDS unit_tests
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
