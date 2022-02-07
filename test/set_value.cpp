// test/set_value.cpp                                                 -*-C++-*-
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
#include P2300_tuple
#include P2300_type_traits
#include <gtest/gtest.h>

// ----------------------------------------------------------------------------

namespace
{
    template <int> struct arg {};
    struct no_arg {};
    template <int I> struct conv { operator arg<I>() const noexcept { return {}; } };

    template <bool Noexcept, typename... A>
    struct rvalue_receiver
    {
        using Args = std::tuple<std::add_pointer_t<std::remove_reference_t<A>>...>;
        static Args  args;
        Args** called;
        friend void tag_invoke(std::execution::set_value_t, rvalue_receiver&& self, A... a) noexcept(Noexcept) {
            self.args = std::make_tuple(&a...);
            *self.called = &self.args;
        }
    };
    template <bool Noexcept, typename... A>
    typename rvalue_receiver<Noexcept, A...>::Args rvalue_receiver<Noexcept, A...>::args{};

    template <bool Noexcept, typename... A>
    struct lvalue_receiver
    {
        using Args = std::tuple<std::add_pointer_t<std::remove_reference_t<A>>...>;
        static Args  args;
        Args** called;
        friend void tag_invoke(std::execution::set_value_t, lvalue_receiver& self, A... a) noexcept(Noexcept) {
            self.args = std::make_tuple(&a...);
            *self.called = &self.args;
        }
    };
    template <bool Noexcept, typename... A>
    typename lvalue_receiver<Noexcept, A...>::Args lvalue_receiver<Noexcept, A...>::args{};

    template <bool Noexcept, typename... A>
    struct const_receiver
    {
        using Args = std::tuple<std::add_pointer_t<std::remove_reference_t<A>>...>;
        static Args  args;
        Args** called;
        friend void tag_invoke(std::execution::set_value_t, const_receiver const& self, A... a) noexcept(Noexcept) {
            self.args = std::make_tuple(&a...);
            *self.called = &self.args;
        }
    };
    template <bool Noexcept, typename... A>
    typename const_receiver<Noexcept, A...>::Args const_receiver<Noexcept, A...>::args{};
}

// ----------------------------------------------------------------------------

TEST(set_value, test_classes)
{
    static_assert(std::is_convertible_v<conv<1>, arg<1>>);
    // rvalue_receiver
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<true>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<false>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<true, arg<0>>, arg<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<false, arg<0>>, arg<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<true, arg<0>>, conv<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<false, arg<0>>, conv<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<true, arg<0>, arg<1>, arg<2>>, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<false, arg<0>, arg<1>, arg<2>>, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<true, arg<0>, arg<1>, arg<2>>, conv<0>, conv<1>, conv<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<false, arg<0>, arg<1>, arg<2>>, conv<0>, conv<1>, conv<2>>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<true, arg<0>>, no_arg>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<false, arg<0>>, no_arg>);

    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<true>&>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<false>&>);

    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<true> const&>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, rvalue_receiver<false> const&>);

    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<rvalue_receiver<true>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<rvalue_receiver<true, arg<0>>>(), std::declval<arg<0>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<rvalue_receiver<true, arg<0>>>(), std::declval<conv<0>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<rvalue_receiver<true, arg<0>, arg<1>, arg<2>>>(), std::declval<arg<0>>(), std::declval<arg<1>>(), std::declval<arg<2>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<rvalue_receiver<true, arg<0>, arg<1>, arg<2>>>(), std::declval<conv<0>>(), std::declval<conv<1>>(), std::declval<conv<2>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<rvalue_receiver<false>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<rvalue_receiver<false, arg<0>>>(), std::declval<arg<0>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<rvalue_receiver<false, arg<0>>>(), std::declval<conv<0>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<rvalue_receiver<false, arg<0>, arg<1>, arg<2>>>(), std::declval<arg<0>>(), std::declval<arg<1>>(), std::declval<arg<2>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<rvalue_receiver<false, arg<0>, arg<1>, arg<2>>>(), std::declval<conv<0>>(), std::declval<conv<1>>(), std::declval<conv<2>>())));

    // lvalue_receiver
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<true>>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<false>>);

    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<true>&>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<false>&>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<true, arg<0>>&, arg<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<false, arg<0>>&, arg<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<true, arg<0>>&, conv<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<false, arg<0>>&, conv<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<true, arg<0>, arg<1>, arg<2>>&, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<false, arg<0>, arg<1>, arg<2>>&, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<true, arg<0>, arg<1>, arg<2>>&, conv<0>, conv<1>, conv<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<false, arg<0>, arg<1>, arg<2>>&, conv<0>, conv<1>, conv<2>>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<true, arg<0>>&, no_arg>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<false, arg<0>>&, no_arg>);

    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<true> const&>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, lvalue_receiver<false> const&>);

    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<lvalue_receiver<true>&>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<lvalue_receiver<true, arg<0>>&>(), std::declval<arg<0>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<lvalue_receiver<true, arg<0>>&>(), std::declval<conv<0>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<lvalue_receiver<true, arg<0>, arg<1>, arg<2>>&>(), std::declval<arg<0>>(), std::declval<arg<1>>(), std::declval<arg<2>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<lvalue_receiver<true, arg<0>, arg<1>, arg<2>>&>(), std::declval<conv<0>>(), std::declval<conv<1>>(), std::declval<conv<2>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<lvalue_receiver<false>&>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<lvalue_receiver<false, arg<0>>&>(), std::declval<arg<0>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<lvalue_receiver<false, arg<0>>&>(), std::declval<conv<0>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<lvalue_receiver<false, arg<0>, arg<1>, arg<2>>&>(), std::declval<arg<0>>(), std::declval<arg<1>>(), std::declval<arg<2>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<lvalue_receiver<false, arg<0>, arg<1>, arg<2>>&>(), std::declval<conv<0>>(), std::declval<conv<1>>(), std::declval<conv<2>>())));

    // const_receiver
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>>, arg<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>>, arg<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>>, conv<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>>, conv<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>, arg<1>, arg<2>>, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>, arg<1>, arg<2>>, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>, arg<1>, arg<2>>, conv<0>, conv<1>, conv<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>, arg<1>, arg<2>>, conv<0>, conv<1>, conv<2>>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>>, no_arg>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>>, no_arg>);

    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true>&>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false>&>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>>&, arg<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>>&, arg<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>>&, conv<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>>&, conv<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>, arg<1>, arg<2>>&, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>, arg<1>, arg<2>>&, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>, arg<1>, arg<2>>&, conv<0>, conv<1>, conv<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>, arg<1>, arg<2>>&, conv<0>, conv<1>, conv<2>>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>>&, no_arg>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>>&, no_arg>);

    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true> const&>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false> const&>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>> const&, arg<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>> const&, arg<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>> const&, conv<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>> const&, conv<0>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>, arg<1>, arg<2>> const&, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>, arg<1>, arg<2>> const&, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>, arg<1>, arg<2>> const&, conv<0>, conv<1>, conv<2>>);
    static_assert(std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>, arg<1>, arg<2>> const&, conv<0>, conv<1>, conv<2>>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<true, arg<0>> const&, no_arg>);
    static_assert(not std::invocable<decltype(std::tag_invoke), std::execution::set_value_t, const_receiver<false, arg<0>> const&, no_arg>);

    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true, arg<0>>>(), std::declval<arg<0>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true, arg<0>>>(), std::declval<conv<0>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true, arg<0>, arg<1>, arg<2>>>(), std::declval<arg<0>>(), std::declval<arg<1>>(), std::declval<arg<2>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true, arg<0>, arg<1>, arg<2>>>(), std::declval<conv<0>>(), std::declval<conv<1>>(), std::declval<conv<2>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<false>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<false, arg<0>>>(), std::declval<arg<0>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<false, arg<0>>>(), std::declval<conv<0>>())));

    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true>&>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true, arg<0>>&>(), std::declval<arg<0>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true, arg<0>>&>(), std::declval<conv<0>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true, arg<0>, arg<1>, arg<2>>&>(), std::declval<arg<0>>(), std::declval<arg<1>>(), std::declval<arg<2>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true, arg<0>, arg<1>, arg<2>>&>(), std::declval<conv<0>>(), std::declval<conv<1>>(), std::declval<conv<2>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<false>&>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<false, arg<0>>&>(), std::declval<arg<0>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<false, arg<0>>&>(), std::declval<conv<0>>())));

    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true> const&>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true, arg<0>> const&>(), std::declval<arg<0>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true, arg<0>> const&>(), std::declval<conv<0>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true, arg<0>, arg<1>, arg<2>> const&>(), std::declval<arg<0>>(), std::declval<arg<1>>(), std::declval<arg<2>>())));
    static_assert(noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<true, arg<0>, arg<1>, arg<2>> const&>(), std::declval<conv<0>>(), std::declval<conv<1>>(), std::declval<conv<2>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<false> const&>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<false, arg<0>> const&>(), std::declval<arg<0>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<false, arg<0>> const&>(), std::declval<conv<0>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<false, arg<0>, arg<1>, arg<2>> const&>(), std::declval<arg<0>>(), std::declval<arg<1>>(), std::declval<arg<2>>())));
    static_assert(not noexcept(tag_invoke(std::execution::set_value, std::declval<const_receiver<false, arg<0>, arg<1>, arg<2>> const&>(), std::declval<conv<0>>(), std::declval<conv<1>>(), std::declval<conv<2>>())));
}

TEST(set_value, rvalue_calls)
{
    static_assert(std::invocable<std::execution::set_value_t const, rvalue_receiver<true>>);
    static_assert(std::invocable<std::execution::set_value_t const, rvalue_receiver<true, arg<0>>, arg<0>>);
    static_assert(std::invocable<std::execution::set_value_t const, rvalue_receiver<true, arg<0>>, conv<0>>);
    static_assert(not std::invocable<std::execution::set_value_t const, rvalue_receiver<true, arg<0>>, no_arg>);
    static_assert(std::invocable<std::execution::set_value_t const, rvalue_receiver<true, arg<0>, arg<1>, arg<2>>, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<std::execution::set_value_t const, rvalue_receiver<true, arg<0>, arg<1>, arg<2>>, conv<0>, conv<1>, conv<2>>);
    static_assert(not std::invocable<std::execution::set_value_t const, rvalue_receiver<false>>);
    static_assert(not std::invocable<std::execution::set_value_t const, rvalue_receiver<true>&>);
    static_assert(not std::invocable<std::execution::set_value_t const, rvalue_receiver<false>&>);
    static_assert(not std::invocable<std::execution::set_value_t const, rvalue_receiver<true> const&>);
    static_assert(not std::invocable<std::execution::set_value_t const, rvalue_receiver<false> const&>);
    static_assert(noexcept(std::execution::set_value(std::declval<rvalue_receiver<true>>())));

    {
        std::tuple<>* called{nullptr};
        ASSERT_EQ(called, nullptr);
        std::execution::set_value(rvalue_receiver<true>{&called});
        EXPECT_NE(called, nullptr);
    }
    {
        std::tuple<arg<0>*>* called{nullptr};
        arg<0>               arg0{};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(rvalue_receiver<true, arg<0>&>{&called}, arg0);
        EXPECT_NE(called, nullptr);
        EXPECT_EQ(*called, std::make_tuple(&arg0));
    }
    {
        std::tuple<arg<0>*>* called{nullptr};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(rvalue_receiver<true, arg<0>>{&called}, conv<0>{});
        EXPECT_NE(called, nullptr);
    }
    {
        std::tuple<arg<0>*, arg<1>*, arg<2>*>* called{nullptr};
        arg<0>                                 arg0{};
        arg<1>                                 arg1{};
        arg<2>                                 arg2{};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(rvalue_receiver<true, arg<0>&, arg<1>&, arg<2>&>{&called}, arg0, arg1, arg2);
        EXPECT_NE(called, nullptr);
        EXPECT_EQ(*called, std::make_tuple(&arg0, &arg1, &arg2));
    }
    {
        std::tuple<arg<0>*, arg<1>*, arg<2>*>* called{nullptr};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(rvalue_receiver<true, arg<0>, arg<1>, arg<2>>{&called}, conv<0>{}, conv<1>{}, conv<2>{});
        EXPECT_NE(called, nullptr);
    }
}

TEST(set_value, lvalue_calls)
{
    static_assert(not std::invocable<std::execution::set_value_t const, lvalue_receiver<true>>);
    static_assert(not std::invocable<std::execution::set_value_t const, lvalue_receiver<false>>);

    static_assert(std::invocable<std::execution::set_value_t const, lvalue_receiver<true>&>);
    static_assert(std::invocable<std::execution::set_value_t const, lvalue_receiver<true, arg<0>>&, arg<0>>);
    static_assert(std::invocable<std::execution::set_value_t const, lvalue_receiver<true, arg<0>>&, conv<0>>);
    static_assert(not std::invocable<std::execution::set_value_t const, lvalue_receiver<true, arg<0>>&, no_arg>);
    static_assert(std::invocable<std::execution::set_value_t const, lvalue_receiver<true, arg<0>, arg<1>, arg<2>>&, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<std::execution::set_value_t const, lvalue_receiver<true, arg<0>, arg<1>, arg<2>>&, conv<0>, conv<1>, conv<2>>);
    static_assert(not std::invocable<std::execution::set_value_t const, lvalue_receiver<false>&>);
    static_assert(not std::invocable<std::execution::set_value_t const, lvalue_receiver<false, arg<0>>&, arg<0>>);
    static_assert(not std::invocable<std::execution::set_value_t const, lvalue_receiver<false, arg<0>>&, conv<0>>);
    static_assert(not std::invocable<std::execution::set_value_t const, lvalue_receiver<false, arg<0>,arg<1>, arg<2>>&, arg<0>,arg<1>, arg<2>>);
    static_assert(not std::invocable<std::execution::set_value_t const, lvalue_receiver<false, arg<0>,arg<1>, arg<2>>&, conv<0>,conv<1>, conv<2>>);

    static_assert(not std::invocable<std::execution::set_value_t const, lvalue_receiver<true> const&>);
    static_assert(not std::invocable<std::execution::set_value_t const, lvalue_receiver<false> const&>);

    static_assert(noexcept(std::execution::set_value(std::declval<lvalue_receiver<true>&>())));
    static_assert(noexcept(std::execution::set_value(std::declval<lvalue_receiver<true, arg<0>&>&>(), std::declval<arg<0>&>())));
    static_assert(noexcept(std::execution::set_value(std::declval<lvalue_receiver<true, arg<0>>&>(), std::declval<conv<0>>())));
    static_assert(noexcept(std::execution::set_value(std::declval<lvalue_receiver<true, arg<0>&, arg<1>&, arg<2>&>&>(), std::declval<arg<0>&>(), std::declval<arg<1>&>(), std::declval<arg<2>&>())));
    static_assert(noexcept(std::execution::set_value(std::declval<lvalue_receiver<true, arg<0>, arg<1>, arg<2>>&>(), std::declval<conv<0>>(), std::declval<conv<1>>(), std::declval<conv<2>>())));

    {
        std::tuple<>*         called{nullptr};
        lvalue_receiver<true> receiver{&called};
        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver);
        EXPECT_NE(called, nullptr);
    }
    {
        std::tuple<arg<0>*>*           called{nullptr};
        lvalue_receiver<true, arg<0>&> receiver{&called};
        arg<0>                         arg0{};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver, arg0);
        EXPECT_NE(called, nullptr);
        EXPECT_EQ(*called, std::make_tuple(&arg0));
    }
    {
        std::tuple<arg<0>*>*          called{nullptr};
        lvalue_receiver<true, arg<0>> receiver{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver, conv<0>{});
        EXPECT_NE(called, nullptr);
    }
    {
        std::tuple<arg<0>*, arg<1>*, arg<2>*>*           called{nullptr};
        lvalue_receiver<true, arg<0>&, arg<1>&, arg<2>&> receiver{&called};
        arg<0>                                           arg0{};
        arg<1>                                           arg1{};
        arg<2>                                           arg2{};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver, arg0, arg1, arg2);
        EXPECT_NE(called, nullptr);
        EXPECT_EQ(*called, std::make_tuple(&arg0, &arg1, &arg2));
    }
    {
        std::tuple<arg<0>*, arg<1>*, arg<2>*>*        called{nullptr};
        lvalue_receiver<true, arg<0>, arg<1>, arg<2>> receiver{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver, conv<0>{}, conv<1>{}, conv<2>{});
        EXPECT_NE(called, nullptr);
    }
}

// ----------------------------------------------------------------------------

TEST(set_value, const_calls)
{
    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true>>);
    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true, arg<0>>, arg<0>>);
    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true, arg<0>>, conv<0>>);
    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true, arg<0>, arg<1>, arg<2>>, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true, arg<0>, arg<1>, arg<2>>, conv<0>, conv<1>, conv<2>>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false>>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false, arg<0>>, arg<0>>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false, arg<0>>, conv<0>>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false, arg<0>, arg<1>, arg<2>>, arg<0>, arg<1>, arg<2>>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false, arg<0>, arg<1>, arg<2>>, conv<0>, conv<1>, conv<2>>);

    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true>&>);
    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true, arg<0>>&, arg<0>>);
    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true, arg<0>>&, conv<0>>);
    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true, arg<0>, arg<1>, arg<2>>&, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true, arg<0>, arg<1>, arg<2>>&, conv<0>, conv<1>, conv<2>>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false>&>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false, arg<0>>&, arg<0>>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false, arg<0>>&, conv<0>>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false, arg<0>, arg<1>, arg<2>>&, arg<0>, arg<1>, arg<2>>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false, arg<0>, arg<1>, arg<2>>&, conv<0>, conv<1>, conv<2>>);

    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true> const&>);
    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true, arg<0>> const&, arg<0>>);
    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true, arg<0>> const&, conv<0>>);
    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true, arg<0>, arg<1>, arg<2>> const&, arg<0>, arg<1>, arg<2>>);
    static_assert(std::invocable<std::execution::set_value_t const, const_receiver<true, arg<0>, arg<1>, arg<2>> const&, conv<0>, conv<1>, conv<2>>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false> const&>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false, arg<0>> const&, arg<0>>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false, arg<0>> const&, conv<0>>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false, arg<0>, arg<1>, arg<2>> const&, arg<0>, arg<1>, arg<2>>);
    static_assert(not std::invocable<std::execution::set_value_t const, const_receiver<false, arg<0>, arg<1>, arg<2>> const&, conv<0>, conv<1>, conv<2>>);

    static_assert(noexcept(std::execution::set_value(std::declval<const_receiver<true>>())));
    static_assert(noexcept(std::execution::set_value(std::declval<const_receiver<true, arg<0>>>(), std::declval<arg<0>>())));
    static_assert(noexcept(std::execution::set_value(std::declval<const_receiver<true, arg<0>>>(), std::declval<conv<0>>())));
    static_assert(noexcept(std::execution::set_value(std::declval<const_receiver<true, arg<0>, arg<1>, arg<2>>>(), std::declval<arg<0>>(), std::declval<arg<1>>(), std::declval<arg<2>>())));
    static_assert(noexcept(std::execution::set_value(std::declval<const_receiver<true, arg<0>, arg<1>, arg<2>>>(), std::declval<conv<0>>(), std::declval<conv<1>>(), std::declval<conv<2>>())));

    static_assert(noexcept(std::execution::set_value(std::declval<const_receiver<true>&>())));
    static_assert(noexcept(std::execution::set_value(std::declval<const_receiver<true, arg<0>>&>(), std::declval<arg<0>>())));
    static_assert(noexcept(std::execution::set_value(std::declval<const_receiver<true, arg<0>>&>(), std::declval<conv<0>>())));
    static_assert(noexcept(std::execution::set_value(std::declval<const_receiver<true, arg<0>, arg<1>, arg<2>>&>(), std::declval<arg<0>>(), std::declval<arg<1>>(), std::declval<arg<2>>())));
    static_assert(noexcept(std::execution::set_value(std::declval<const_receiver<true, arg<0>, arg<1>, arg<2>>&>(), std::declval<conv<0>>(), std::declval<conv<1>>(), std::declval<conv<2>>())));

    {
        std::tuple<>* called{nullptr};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(const_receiver<true>{&called});
        EXPECT_NE(called, nullptr);
    }
    {
        std::tuple<arg<0>*>* called{nullptr};
        arg<0>               arg0{};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(const_receiver<true, arg<0>&>{&called}, arg0);
        EXPECT_NE(called, nullptr);
        EXPECT_EQ(*called, std::make_tuple(&arg0));
    }
    {
        std::tuple<arg<0>*>* called{nullptr};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(const_receiver<true, arg<0>>{&called}, conv<0>{});
        EXPECT_NE(called, nullptr);
    }
    {
        std::tuple<arg<0>*, arg<1>*, arg<2>*>* called{nullptr};
        arg<0>                                 arg0{};
        arg<1>                                 arg1{};
        arg<2>                                 arg2{};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(const_receiver<true, arg<0>&, arg<1>&, arg<2>&>{&called}, arg0, arg1, arg2);
        EXPECT_NE(called, nullptr);
        EXPECT_EQ(*called, std::make_tuple(&arg0, &arg1, &arg2));
    }
    {
        std::tuple<arg<0>*, arg<1>*, arg<2>*>* called{nullptr};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(const_receiver<true, arg<0>, arg<1>, arg<2>>{&called}, conv<0>{}, conv<1>{}, conv<2>{});
        EXPECT_NE(called, nullptr);
    }

    {
        std::tuple<>*        called{nullptr};
        const_receiver<true> receiver{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver);
        EXPECT_NE(called, nullptr);
    }
    {
        std::tuple<arg<0>*>*          called{nullptr};
        const_receiver<true, arg<0>&> receiver{&called};
        arg<0>                        arg0{};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver, arg0);
        EXPECT_NE(called, nullptr);
        EXPECT_EQ(*called, std::make_tuple(&arg0));
    }
    {
        std::tuple<arg<0>*>*         called{nullptr};
        const_receiver<true, arg<0>> receiver{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver, conv<0>{});
        EXPECT_NE(called, nullptr);
    }
    {
        std::tuple<arg<0>*, arg<1>*, arg<2>*>*          called{nullptr};
        const_receiver<true, arg<0>&, arg<1>&, arg<2>&> receiver{&called};
        arg<0>                                          arg0{};
        arg<1>                                          arg1{};
        arg<2>                                          arg2{};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver, arg0, arg1, arg2);
        EXPECT_NE(called, nullptr);
        EXPECT_EQ(*called, std::make_tuple(&arg0, &arg1, &arg2));
    }
    {
        std::tuple<arg<0>*, arg<1>*, arg<2>*>*       called{nullptr};
        const_receiver<true, arg<0>, arg<1>, arg<2>> receiver{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver, conv<0>{}, conv<1>{}, conv<2>{});
        EXPECT_NE(called, nullptr);
    }

    {
        std::tuple<>*              called{nullptr};
        const_receiver<true> const receiver{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver);
        EXPECT_NE(called, nullptr);
    }
    {
        std::tuple<arg<0>*>*                called{nullptr};
        const_receiver<true, arg<0>&> const receiver{&called};
        arg<0>                              arg0{};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver, arg0);
        EXPECT_NE(called, nullptr);
        EXPECT_EQ(*called, std::make_tuple(&arg0));
    }
    {
        std::tuple<arg<0>*>*               called{nullptr};
        const_receiver<true, arg<0>> const receiver{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver, conv<0>{});
        EXPECT_NE(called, nullptr);
    }
    {
        std::tuple<arg<0>*, arg<1>*, arg<2>*>*                called{nullptr};
        const_receiver<true, arg<0>&, arg<1>&, arg<2>&> const receiver{&called};
        arg<0>                                                arg0{};
        arg<1>                                                arg1{};
        arg<2>                                                arg2{};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver, arg0, arg1, arg2);
        EXPECT_NE(called, nullptr);
        EXPECT_EQ(*called, std::make_tuple(&arg0, &arg1, &arg2));
    }
    {
        std::tuple<arg<0>*, arg<1>*, arg<2>*>*             called{nullptr};
        const_receiver<true, arg<0>, arg<1>, arg<2>> const receiver{&called};

        ASSERT_EQ(called, nullptr);
        std::execution::set_value(receiver, conv<0>{}, conv<1>{}, conv<2>{});
        EXPECT_NE(called, nullptr);
    }
}