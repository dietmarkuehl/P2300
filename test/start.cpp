// test/start.cpp                                                     -*-C++-*-
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

namespace {
    template <bool Noexcept>
    struct state {
        int* d_started;
        friend void tag_invoke(std::execution::start_t, state&&) noexcept(Noexcept) {
        }
        friend void tag_invoke(std::execution::start_t, state& self) noexcept(Noexcept) {
            *self.d_started = 1;
        }
        friend void tag_invoke(std::execution::start_t, state const& self) noexcept(Noexcept) {
            *self.d_started = 2;
        }
    };

    class indestructible_state {
    private:
        int* d_started;
        ~indestructible_state() {}

    public:
        friend void tag_invoke(std::execution::start_t, indestructible_state& self) noexcept {
            *self.d_started = 3;
        }

        indestructible_state(int* started): d_started(started) {}
        static void destroy(indestructible_state* state) { delete state; }
    };
}

TEST(start, static_properties_nonthrowing)
{
    // verify test class
    static_assert(noexcept(tag_invoke(std::execution::start, std::declval<state<true>>())));
    static_assert(noexcept(tag_invoke(std::execution::start, std::declval<state<true>&&>())));
    static_assert(noexcept(tag_invoke(std::execution::start, std::declval<state<true>&>())));
    static_assert(noexcept(tag_invoke(std::execution::start, std::declval<state<true> const&>())));

    // rvalues can't be used via the start CPO
    static_assert(not std::invocable<std::execution::start_t, state<true>>);
    static_assert(not std::invocable<std::execution::start_t, state<true>&&>);

    // lvalues can be used via the start CPO and don't throw
    static_assert(noexcept(std::tag_invoke(std::execution::start, std::declval<state<true>&>())));
    static_assert(noexcept(std::tag_invoke(std::execution::start, std::declval<state<true> const&>())));
    static_assert(std::is_nothrow_invocable_v<std::execution::start_t, state<true>&>);
    static_assert(std::is_nothrow_invocable_v<std::execution::start_t, state<true> const&>);
}

TEST(start, static_properties_throwing)
{
    // verify test class
    static_assert(not noexcept(tag_invoke(std::execution::start, std::declval<state<false>>())));
    static_assert(not noexcept(tag_invoke(std::execution::start, std::declval<state<false>&&>())));
    static_assert(not noexcept(tag_invoke(std::execution::start, std::declval<state<false>&>())));
    static_assert(not noexcept(tag_invoke(std::execution::start, std::declval<state<false> const&>())));

    // none of the operations work via the start CPO if they'd throw
    static_assert(not std::invocable<std::execution::start_t, state<false>>);
    static_assert(not std::invocable<std::execution::start_t, state<false>&&>);
    static_assert(not std::invocable<std::execution::start_t, state<false>&>);
    static_assert(not std::invocable<std::execution::start_t, state<false> const&>);
}

TEST(start, non_const_behavior)
{
    int         started{0};
    state<true> s{&started};

    ASSERT_EQ(started, 0);
    std::execution::start(s);
    EXPECT_EQ(started, 1);
}

TEST(start, const_behavior)
{
    int               started{0};
    state<true> const s{&started};

    ASSERT_EQ(started, 0);
    std::execution::start(s);
    EXPECT_EQ(started, 2);
}

TEST(start, indestructible_state)
{
    static_assert(noexcept(tag_invoke(std::execution::start, std::declval<indestructible_state&>())));
    static_assert(noexcept(std::tag_invoke(std::execution::start, std::declval<indestructible_state&>())));
    static_assert(std::is_nothrow_invocable_v<std::execution::start_t, indestructible_state&>);

    int   started{0};
    auto* s(new indestructible_state(&started));

    ASSERT_EQ(started, 0);
    std::execution::start(*s);
    EXPECT_EQ(started, 3);

    indestructible_state::destroy(s);
}