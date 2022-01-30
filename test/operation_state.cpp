// test/operation_state.hpp                                           -*-C++-*-
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
        friend void tag_invoke(std::execution::start_t, state& self) noexcept(Noexcept);
        friend void tag_invoke(std::execution::start_t, state const& self) noexcept(Noexcept);
    };

    template <bool> // to prevent a compiler warning
    struct indestructible_state {
        friend void tag_invoke(std::execution::start_t, indestructible_state& self) noexcept;
    private:
        ~indestructible_state();
    };
}

TEST(start, breathing)
{
    static_assert(std::execution::operation_state<state<true>>);
    static_assert(std::execution::operation_state<state<true> const>);
    static_assert(not std::execution::operation_state<state<true>&>);
    static_assert(not std::execution::operation_state<state<true> const&>);

    static_assert(not std::execution::operation_state<state<false>>);
    static_assert(not std::execution::operation_state<state<false> const>);

    static_assert(std::invocable<std::execution::start_t, indestructible_state<true>&>);
    static_assert(not std::execution::operation_state<indestructible_state<true>>);
}