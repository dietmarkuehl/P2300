// test/set_error.cpp                                                 -*-C++-*-
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
    struct error {};
    struct no_error {};
    struct conv { operator error() const noexcept { return {}; } };

    template <bool Noexcept>
    struct rvalue_receiver
    {
        error const** called;
        friend void tag_invoke(std::execution::set_error_t, rvalue_receiver&& self, error const& e) noexcept(Noexcept) {
            *self.called = &e;
        }
    };

    template <bool Noexcept>
    struct lvalue_receiver
    {
        error const** called;
        friend void tag_invoke(std::execution::set_error_t, lvalue_receiver& self, error const& e) noexcept(Noexcept) {
            *self.called = &e;
        }
    };

    template <bool Noexcept>
    struct const_receiver
    {
        error const** called;
        friend void tag_invoke(std::execution::set_error_t, const_receiver const& self, error const& e) noexcept(Noexcept) {
            *self.called = &e;
        }
    };
}

// ----------------------------------------------------------------------------

TEST(set_error, test_classes)
{
    static_assert(std::is_convertible_v<conv, error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, rvalue_receiver<true>, error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, rvalue_receiver<false>, error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, rvalue_receiver<true>, conv>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, rvalue_receiver<false>, conv>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, rvalue_receiver<true>, no_error>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, rvalue_receiver<false>, no_error>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, rvalue_receiver<true>&, error>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, rvalue_receiver<false>&, error>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, rvalue_receiver<true> const&, error>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, rvalue_receiver<false> const&, error>);

    static_assert(noexcept(tag_invoke(std::execution::set_error, std::declval<rvalue_receiver<true>>(), std::declval<error>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_error, std::declval<rvalue_receiver<false>>(), std::declval<error>())));
    static_assert(noexcept(tag_invoke(std::execution::set_error, std::declval<rvalue_receiver<true>>(), std::declval<conv>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_error, std::declval<rvalue_receiver<false>>(), std::declval<conv>())));

    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, lvalue_receiver<true>, error>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, lvalue_receiver<false>, error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, lvalue_receiver<true>&, error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, lvalue_receiver<false>&, error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, lvalue_receiver<true>&, conv>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, lvalue_receiver<false>&, conv>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, lvalue_receiver<true>&, no_error>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, lvalue_receiver<false>&, no_error>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, lvalue_receiver<true> const&, error>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, lvalue_receiver<false> const&, error>);

    static_assert(noexcept(tag_invoke(std::execution::set_error, std::declval<lvalue_receiver<true>&>(), std::declval<error>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_error, std::declval<lvalue_receiver<false>&>(), std::declval<error>())));
    static_assert(noexcept(tag_invoke(std::execution::set_error, std::declval<lvalue_receiver<true>&>(), std::declval<conv>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_error, std::declval<lvalue_receiver<false>&>(), std::declval<conv>())));

    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<true>, error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<false>, error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<true>, conv>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<false>, conv>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<true>, no_error>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<false>, no_error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<true>&, error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<false>&, error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<true>&, conv>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<false>&, conv>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<true>&, no_error>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<false>&, no_error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<true> const&, error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<false> const&, error>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<true> const&, conv>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<false> const&, conv>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<true> const&, no_error>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_error_t, const_receiver<false> const&, no_error>);

    static_assert(noexcept(tag_invoke(std::execution::set_error, std::declval<const_receiver<true>>(), std::declval<error>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_error, std::declval<const_receiver<false>>(), std::declval<error>())));
    static_assert(noexcept(tag_invoke(std::execution::set_error, std::declval<const_receiver<true>>(), std::declval<conv>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_error, std::declval<const_receiver<false>>(), std::declval<conv>())));
    static_assert(noexcept(tag_invoke(std::execution::set_error, std::declval<const_receiver<true>&>(), std::declval<error>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_error, std::declval<const_receiver<false>&>(), std::declval<error>())));
    static_assert(noexcept(tag_invoke(std::execution::set_error, std::declval<const_receiver<true>&>(), std::declval<conv>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_error, std::declval<const_receiver<false>&>(), std::declval<conv>())));
    static_assert(noexcept(tag_invoke(std::execution::set_error, std::declval<const_receiver<true> const&>(), std::declval<error>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_error, std::declval<const_receiver<false> const&>(), std::declval<error>())));
    static_assert(noexcept(tag_invoke(std::execution::set_error, std::declval<const_receiver<true> const&>(), std::declval<conv>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_error, std::declval<const_receiver<false> const&>(), std::declval<conv>())));
}

TEST(set_error, rvalue_calls)
{
    static_assert(std::invocable<std::execution::set_error_t const, rvalue_receiver<true>, error>);
    static_assert(std::invocable<std::execution::set_error_t const, rvalue_receiver<true>, conv>);
    static_assert(not std::invocable<std::execution::set_error_t const, rvalue_receiver<true>, no_error>);
    static_assert(not std::invocable<std::execution::set_error_t const, rvalue_receiver<false>, error>);
    static_assert(not std::invocable<std::execution::set_error_t const, rvalue_receiver<false>, conv>);
    static_assert(not std::invocable<std::execution::set_error_t const, rvalue_receiver<true>&, error>);
    static_assert(not std::invocable<std::execution::set_error_t const, rvalue_receiver<false>&, error>);
    static_assert(not std::invocable<std::execution::set_error_t const, rvalue_receiver<true> const&, error>);
    static_assert(not std::invocable<std::execution::set_error_t const, rvalue_receiver<false> const&, error>);
    static_assert(noexcept(std::execution::set_error(std::declval<rvalue_receiver<true>>(), std::declval<error>())));
    static_assert(noexcept(std::execution::set_error(std::declval<rvalue_receiver<true>>(), std::declval<conv>())));

    {
        error const* called{nullptr};
        error        e{};

        ASSERT_EQ(called, nullptr);
        std::execution::set_error(rvalue_receiver<true>{&called}, e);
        EXPECT_EQ(called, &e);
    }
    {
        error const* called{nullptr};

        ASSERT_EQ(called, nullptr);
        std::execution::set_error(rvalue_receiver<true>{&called}, conv{});
        EXPECT_NE(called, nullptr);
    }
}

TEST(set_error, lvalue_calls)
{
    static_assert(not std::invocable<std::execution::set_error_t const, lvalue_receiver<true>, error>);
    static_assert(not std::invocable<std::execution::set_error_t const, lvalue_receiver<false>, error>);
    static_assert(std::invocable<std::execution::set_error_t const, lvalue_receiver<true>&, error>);
    static_assert(std::invocable<std::execution::set_error_t const, lvalue_receiver<true>&, conv>);
    static_assert(not std::invocable<std::execution::set_error_t const, lvalue_receiver<true>&, no_error>);
    static_assert(not std::invocable<std::execution::set_error_t const, lvalue_receiver<false>&, error>);
    static_assert(not std::invocable<std::execution::set_error_t const, lvalue_receiver<true> const&, error>);
    static_assert(not std::invocable<std::execution::set_error_t const, lvalue_receiver<false> const&, error>);
    static_assert(noexcept(std::execution::set_error(std::declval<lvalue_receiver<true>&>(), std::declval<error>())));
    static_assert(noexcept(std::execution::set_error(std::declval<lvalue_receiver<true>&>(), std::declval<conv>())));

    {
        error const*          called{nullptr};
        error                 e{};
        lvalue_receiver<true> r{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_error(r, e);
        EXPECT_EQ(called, &e);
    }
    {
        error const*          called{nullptr};
        lvalue_receiver<true> r{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_error(r, conv{});
        EXPECT_NE(called, nullptr);
    }
}

TEST(set_error, const_calls)
{
    static_assert(std::invocable<std::execution::set_error_t const, const_receiver<true>, error>);
    static_assert(std::invocable<std::execution::set_error_t const, const_receiver<true>, conv>);
    static_assert(not std::invocable<std::execution::set_error_t const, const_receiver<true>, no_error>);
    static_assert(not std::invocable<std::execution::set_error_t const, const_receiver<false>, error>);
    static_assert(not std::invocable<std::execution::set_error_t const, const_receiver<false>, conv>);
    static_assert(std::invocable<std::execution::set_error_t const, const_receiver<true>&, error>);
    static_assert(std::invocable<std::execution::set_error_t const, const_receiver<true>&, conv>);
    static_assert(not std::invocable<std::execution::set_error_t const, const_receiver<true>&, no_error>);
    static_assert(not std::invocable<std::execution::set_error_t const, const_receiver<false>&, error>);
    static_assert(not std::invocable<std::execution::set_error_t const, const_receiver<false>&, conv>);
    static_assert(std::invocable<std::execution::set_error_t const, const_receiver<true> const&, error>);
    static_assert(std::invocable<std::execution::set_error_t const, const_receiver<true> const&, conv>);
    static_assert(not std::invocable<std::execution::set_error_t const, const_receiver<true>&, no_error>);
    static_assert(not std::invocable<std::execution::set_error_t const, const_receiver<false> const&, error>);
    static_assert(not std::invocable<std::execution::set_error_t const, const_receiver<false> const&, conv>);
    static_assert(noexcept(std::execution::set_error(std::declval<const_receiver<true>>(), std::declval<error>())));
    static_assert(noexcept(std::execution::set_error(std::declval<const_receiver<true>>(), std::declval<conv>())));
    static_assert(noexcept(std::execution::set_error(std::declval<const_receiver<true>&>(), std::declval<error>())));
    static_assert(noexcept(std::execution::set_error(std::declval<const_receiver<true>&>(), std::declval<conv>())));
    static_assert(noexcept(std::execution::set_error(std::declval<const_receiver<true> const&>(), std::declval<error>())));
    static_assert(noexcept(std::execution::set_error(std::declval<const_receiver<true> const&>(), std::declval<conv>())));

    {
        error const*          called{nullptr};
        error                 e{};

        ASSERT_EQ(called, nullptr);
        std::execution::set_error(const_receiver<true>{&called}, e);
        EXPECT_EQ(called, &e);
    }
    {
        error const*          called{nullptr};

        ASSERT_EQ(called, nullptr);
        std::execution::set_error(const_receiver<true>{&called}, conv{});
        EXPECT_NE(called, nullptr);
    }
    {
        error const*          called{nullptr};
        error                 e{};
        const_receiver<true> r{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_error(r, e);
        EXPECT_EQ(called, &e);
    }
    {
        error const*          called{nullptr};
        const_receiver<true> r{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_error(r, conv{});
        EXPECT_NE(called, nullptr);
    }
    {
        error const*               called{nullptr};
        error                      e{};
        const_receiver<true> const r{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_error(r, e);
        EXPECT_EQ(called, &e);
    }
    {
        error const*               called{nullptr};
        const_receiver<true> const r{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_error(r, conv{});
        EXPECT_NE(called, nullptr);
    }
}