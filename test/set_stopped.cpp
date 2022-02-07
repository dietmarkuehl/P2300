// test/set_stopped.cpp                                               -*-C++-*-
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

// ----------------------------------------------------------------------------

namespace
{
    template <bool Noexcept>
    struct rvalue_receiver
    {
        bool* called;
        friend void tag_invoke(std::execution::set_stopped_t, rvalue_receiver&& self) noexcept(Noexcept) {
            *self.called = true;
        }
    };

    template <bool Noexcept>
    struct lvalue_receiver
    {
        bool* called;
        friend void tag_invoke(std::execution::set_stopped_t, lvalue_receiver& self) noexcept(Noexcept) {
            *self.called = true;
        }
    };

    template <bool Noexcept>
    struct const_receiver
    {
        bool* called;
        friend void tag_invoke(std::execution::set_stopped_t, const_receiver const& self) noexcept(Noexcept) {
            *self.called = true;
        }
    };
}

// ----------------------------------------------------------------------------

TEST(set_stopped, test_classes)
{
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, rvalue_receiver<true>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, rvalue_receiver<false>>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, rvalue_receiver<true>&>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, rvalue_receiver<false>&>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, rvalue_receiver<true> const&>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, rvalue_receiver<false> const&>);
    static_assert(noexcept(tag_invoke(std::execution::set_stopped, std::declval<rvalue_receiver<true>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_stopped, std::declval<rvalue_receiver<false>>())));

    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, lvalue_receiver<true>>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, lvalue_receiver<false>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, lvalue_receiver<true>&>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, lvalue_receiver<false>&>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, lvalue_receiver<true> const&>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, lvalue_receiver<false> const&>);
    static_assert(noexcept(tag_invoke(std::execution::set_stopped, std::declval<lvalue_receiver<true>&>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_stopped, std::declval<lvalue_receiver<false>&>())));

    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, const_receiver<true>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, const_receiver<false>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, const_receiver<true>&>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, const_receiver<false>&>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, const_receiver<true> const&>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_stopped_t, const_receiver<false> const&>);
    static_assert(noexcept(tag_invoke(std::execution::set_stopped, std::declval<const_receiver<true>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_stopped, std::declval<const_receiver<false>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_stopped, std::declval<const_receiver<true>&>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_stopped, std::declval<const_receiver<false>&>())));
    static_assert(noexcept(tag_invoke(std::execution::set_stopped, std::declval<const_receiver<true> const&>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_stopped, std::declval<const_receiver<false> const&>())));
}

TEST(set_stopped, rvalue_calls)
{
    static_assert(std::invocable<std::execution::set_stopped_t const, rvalue_receiver<true>>);
    static_assert(not std::invocable<std::execution::set_stopped_t const, rvalue_receiver<false>>);
    static_assert(not std::invocable<std::execution::set_stopped_t const, rvalue_receiver<true>&>);
    static_assert(not std::invocable<std::execution::set_stopped_t const, rvalue_receiver<false>&>);
    static_assert(not std::invocable<std::execution::set_stopped_t const, rvalue_receiver<true> const&>);
    static_assert(not std::invocable<std::execution::set_stopped_t const, rvalue_receiver<false> const&>);
    static_assert(noexcept(std::execution::set_stopped(std::declval<rvalue_receiver<true>>())));

    bool called{false};
    ASSERT_FALSE(called);
    std::execution::set_stopped(rvalue_receiver<true>{&called});
    EXPECT_TRUE(called);

}

TEST(set_stopped, lvalue_calls)
{
    static_assert(not std::invocable<std::execution::set_stopped_t const, lvalue_receiver<true>>);
    static_assert(not std::invocable<std::execution::set_stopped_t const, lvalue_receiver<false>>);
    static_assert(std::invocable<std::execution::set_stopped_t const, lvalue_receiver<true>&>);
    static_assert(not std::invocable<std::execution::set_stopped_t const, lvalue_receiver<false>&>);
    static_assert(not std::invocable<std::execution::set_stopped_t const, lvalue_receiver<true> const&>);
    static_assert(not std::invocable<std::execution::set_stopped_t const, lvalue_receiver<false> const&>);
    static_assert(noexcept(std::execution::set_stopped(std::declval<lvalue_receiver<true>&>())));

    bool                  called{false};
    lvalue_receiver<true> receiver{&called};

    ASSERT_FALSE(called);
    std::execution::set_stopped(receiver);
    EXPECT_TRUE(called);

}

TEST(set_stopped, const_calls)
{
    static_assert(std::invocable<std::execution::set_stopped_t const, const_receiver<true>>);
    static_assert(not std::invocable<std::execution::set_stopped_t const, const_receiver<false>>);
    static_assert(std::invocable<std::execution::set_stopped_t const, const_receiver<true>&>);
    static_assert(not std::invocable<std::execution::set_stopped_t const, const_receiver<false>&>);
    static_assert(std::invocable<std::execution::set_stopped_t const, const_receiver<true> const&>);
    static_assert(not std::invocable<std::execution::set_stopped_t const, const_receiver<false> const&>);
    static_assert(noexcept(std::execution::set_stopped(std::declval<const_receiver<true>>())));
    static_assert(noexcept(std::execution::set_stopped(std::declval<const_receiver<true>&>())));
    static_assert(noexcept(std::execution::set_stopped(std::declval<const_receiver<true> const&>())));

    {
        bool called{false};

        ASSERT_FALSE(called);
        std::execution::set_stopped(const_receiver<true>{&called});
        EXPECT_TRUE(called);
    }
    {
        bool                 called{false};
        const_receiver<true> receiver{&called};

        ASSERT_FALSE(called);
        std::execution::set_stopped(receiver);
        EXPECT_TRUE(called);
    }
    {
        bool                       called{false};
        const_receiver<true> const receiver{&called};

        ASSERT_FALSE(called);
        std::execution::set_stopped(receiver);
        EXPECT_TRUE(called);
    }
}