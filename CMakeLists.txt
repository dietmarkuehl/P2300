cmake_minimum_required(VERSION 3.14.0)
project(p2300 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Werror")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Werror")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX")
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_SOURCE_DIR}/std-headers ${CMAKE_SOURCE_DIR}/include)

enable_testing()

list(APPEND test_exposition_only
    test/callable.cpp
    test/nothrow_callable.cpp
    test/call_result_t.cpp
    test/tag_invoke.cpp
    test/movable_value.cpp
    )

list(APPEND test_components
    )

add_executable(p2300_test ${test_exposition_only} ${test_components})
target_link_libraries(p2300_test PUBLIC gtest_main)

include(GoogleTest)
gtest_discover_tests(p2300_test)