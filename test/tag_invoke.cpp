// test/tag_invoke.cpp                                         -*-C++-*-
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
#include P2300_functional
#include P2300_type_traits
#include <gtest/gtest.h>

using namespace std;
// ----------------------------------------------------------------------------

struct whatever {};
struct with_const {};
struct with_reference {};
struct conversion_target {};
struct with_conversion {
    operator conversion_target() noexcept {
        return {};
    }
};
struct with_explicit_conversion {
    explicit operator conversion_target() noexcept {
        return {};
    }
};

struct noexcept_function {};
struct noexcept_observable {};

namespace meow {
    struct meow_tag_t {};
    struct meow_other_tag_t {};

    auto tag_invoke(meow_tag_t, int) -> int;
    auto tag_invoke(meow_tag_t, double) -> double;
    auto tag_invoke(meow_tag_t, whatever, int)->whatever;
    auto tag_invoke(meow_tag_t, const with_const) -> const with_const;
    auto tag_invoke(meow_tag_t, with_reference&)->with_reference&;
    auto tag_invoke(meow_tag_t, conversion_target)->conversion_target;
    auto tag_invoke(meow_tag_t, noexcept_function&&) noexcept -> noexcept_function;
    auto tag_invoke(meow_tag_t, const noexcept_observable) noexcept -> noexcept_observable;

    namespace woof {
        auto tag_invoke(meow_other_tag_t, long) -> int;
    }
} // namespace meow

namespace bark {
    struct with_conversion_other_namespace {
        operator conversion_target() noexcept {
            return {};
        }
    };

    auto tag_invoke(meow::meow_other_tag_t, double) -> int;
} // namespace bark

TEST(tag_invocable, matching_arguments) {
    static_assert(tag_invocable<meow::meow_tag_t, int>);
    static_assert(tag_invocable<meow::meow_tag_t, double>);
    static_assert(tag_invocable<meow::meow_tag_t, whatever, int>);
    static_assert(tag_invocable<meow::meow_tag_t, conversion_target>);
}

TEST(tag_invocable, mismatched_arguments) {
    static_assert(!tag_invocable<meow::meow_tag_t, int, int>);
    static_assert(!tag_invocable<meow::meow_tag_t, double, int>);
    static_assert(!tag_invocable<meow::meow_tag_t, whatever, int, int>);
    static_assert(!tag_invocable<meow::meow_tag_t>);
}

TEST(tag_invocable, with_conversion) {
    static_assert(tag_invocable<meow::meow_tag_t, with_conversion>);
    static_assert(tag_invocable<meow::meow_tag_t, bark::with_conversion_other_namespace>);
    static_assert(!tag_invocable<meow::meow_tag_t, with_explicit_conversion>);
}

TEST(tag_invocable, with_reference) {
    static_assert(!tag_invocable<meow::meow_tag_t, with_reference>);
    static_assert(tag_invocable<meow::meow_tag_t, with_reference&>);
    static_assert(!tag_invocable<meow::meow_tag_t, with_reference&&>);
    static_assert(!tag_invocable<meow::meow_tag_t, const with_reference>);
    static_assert(!tag_invocable<meow::meow_tag_t, const with_reference&>);
    static_assert(!tag_invocable<meow::meow_tag_t, const with_reference&&>);
}

TEST(tag_invocable, with_qualifier) {
    static_assert(tag_invocable<meow::meow_tag_t, const int>);
    static_assert(tag_invocable<meow::meow_tag_t, volatile int>);
    static_assert(tag_invocable<meow::meow_tag_t, const volatile int>);

    static_assert(tag_invocable<meow::meow_tag_t, with_const>);
    static_assert(tag_invocable<meow::meow_tag_t, const with_const>);
    static_assert(!tag_invocable<meow::meow_tag_t, volatile with_const>);
    static_assert(!tag_invocable<meow::meow_tag_t, const volatile with_const>);

    static_assert(tag_invocable<meow::meow_tag_t, with_const&>);
    static_assert(tag_invocable<meow::meow_tag_t, const with_const&>);
    static_assert(!tag_invocable<meow::meow_tag_t, volatile with_const&>);
    static_assert(!tag_invocable<meow::meow_tag_t, const volatile with_const&>);

    static_assert(tag_invocable<meow::meow_tag_t, with_const&&>);
    static_assert(tag_invocable<meow::meow_tag_t, const with_const&&>);
    static_assert(!tag_invocable<meow::meow_tag_t, volatile with_const&&>);
    static_assert(!tag_invocable<meow::meow_tag_t, const volatile with_const&&>);
}

TEST(tag_invocable, references_can_bind_too) {
    static_assert(tag_invocable<meow::meow_tag_t, int&>);
    static_assert(tag_invocable<meow::meow_tag_t, int&&>);
    static_assert(tag_invocable<meow::meow_tag_t, const int&>);
    static_assert(tag_invocable<meow::meow_tag_t, const int&&>);
}

TEST(tag_invocable, non_associated_namespaces)
{
    static_assert(!tag_invocable<meow::meow_other_tag_t, int>);
    static_assert(!tag_invocable<meow::meow_other_tag_t, double>);
    static_assert(!tag_invocable<meow::meow_other_tag_t, long>);
}

TEST(nothrow_tag_invocable, basic_test)
{
    static_assert(!nothrow_tag_invocable<meow::meow_tag_t, int>);
    static_assert(nothrow_tag_invocable<meow::meow_tag_t, noexcept_function>);
}

TEST(_Tag_observable, takes_const_reference_to_cpo) {
    static_assert(!_Tag_observable<meow::meow_tag_t, int>);
    static_assert(!_Tag_observable<meow::meow_tag_t, noexcept_function>);
    static_assert(_Tag_observable<meow::meow_tag_t, noexcept_observable>);
}

TEST(tag_invoke_result_t, proper_return_type) {
    static_assert(is_same_v<tag_invoke_result_t<meow::meow_tag_t, int>, int>);
    static_assert(is_same_v<tag_invoke_result_t<meow::meow_tag_t, double>, double>);
    static_assert(is_same_v<tag_invoke_result_t<meow::meow_tag_t, float>, double>);
    static_assert(is_same_v<tag_invoke_result_t<meow::meow_tag_t, short>, int>);
    static_assert(is_same_v<tag_invoke_result_t<meow::meow_tag_t, conversion_target>, conversion_target>);
    static_assert(is_same_v<tag_invoke_result_t<meow::meow_tag_t, with_conversion>, conversion_target>);
    static_assert(is_same_v<tag_invoke_result_t<meow::meow_tag_t, with_const>, const with_const>);
    static_assert(is_same_v<tag_invoke_result_t<meow::meow_tag_t, with_reference&>, with_reference&>);
}
