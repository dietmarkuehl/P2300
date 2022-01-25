// test/movable_value.cpp                                         -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de
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

struct aggregate
{
};
TEST(movable_value, with_cv_ref)
{
    static_assert(execution::_Movable_value<aggregate>);
    static_assert(execution::_Movable_value<aggregate &>);
    static_assert(execution::_Movable_value<const aggregate &>);
    static_assert(execution::_Movable_value<const aggregate>);
}

struct move_only
{
    move_only(const move_only &) = delete;
    move_only &operator=(const move_only &) = delete;
};
TEST(movable_value, move_only)
{
    static_assert(!execution::_Movable_value<move_only>);
    static_assert(!execution::_Movable_value<move_only &>);
    static_assert(!execution::_Movable_value<const move_only &>);
    static_assert(!execution::_Movable_value<const move_only>);
}

struct copy_only
{
    copy_only(copy_only &&) = delete;
    copy_only &operator=(copy_only &&) = delete;
};
TEST(movable_value, copy_only)
{
    static_assert(!execution::_Movable_value<copy_only>);
    static_assert(!execution::_Movable_value<copy_only &>);
    static_assert(!execution::_Movable_value<const copy_only &>);
    static_assert(!execution::_Movable_value<const copy_only>);
}

struct not_assignable
{
    not_assignable &operator=(const not_assignable &) = delete;
    not_assignable &operator=(not_assignable &&) = delete;
};
TEST(movable_value, not_assignable)
{
    static_assert(!execution::_Movable_value<not_assignable>);
    static_assert(!execution::_Movable_value<not_assignable &>);
    static_assert(!execution::_Movable_value<const not_assignable &>);
    static_assert(!execution::_Movable_value<const not_assignable>);
}

struct not_constructible
{
    not_constructible(const not_constructible &) = delete;
    not_constructible(not_constructible &&) = delete;
};
TEST(movable_value, not_constructible)
{
    static_assert(!execution::_Movable_value<not_constructible>);
    static_assert(!execution::_Movable_value<not_constructible &>);
    static_assert(!execution::_Movable_value<const not_constructible &>);
    static_assert(!execution::_Movable_value<const not_constructible>);
}
