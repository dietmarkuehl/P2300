// test/non_associated.cpp                                         -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
//
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without restriction,
//  including without limitation the rights to use, copy, modify,
//  merge, publish, distribute, sublicense, and/or sell copies of
//  the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "test-config.hpp"
#include P2300_execution
#include P2300_type_traits
#include <gtest/gtest.h>

using namespace std;
// ----------------------------------------------------------------------------

struct meow
{
    template <__non_associated T>
    void operator()(T) {}
};
TEST(non_associated, smoke_test)
{
    static_assert(!is_invocable_v<meow, int>);
    static_assert(is_invocable_v<meow, __unassociate<int>>);
    static_assert(same_as<int, __reassociate<__unassociate<int>>>);
}

namespace non_associated {
    struct woof {};
    constexpr void test_associated(auto);
}

template <typename, class = void>
constexpr bool is_associated = false;
template <typename T>
constexpr bool is_associated<T, void_t<decltype(test_associated(declval<T>()))>> = true;

TEST(non_associated, test_adl)
{
    static_assert(is_associated<non_associated::woof>);
    static_assert(!is_associated<__unassociate<non_associated::woof>>);
}
