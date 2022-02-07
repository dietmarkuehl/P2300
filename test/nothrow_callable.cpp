// test/nothrow_callable.cpp                                          -*-C++-*-
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
#include P2300_functional
#include <gtest/gtest.h>

// ----------------------------------------------------------------------------

namespace td {
    namespace {
        template <bool Throwing>
        struct type {
            type(type const&) noexcept(not Throwing) {}
        };

    }
}

// ----------------------------------------------------------------------------

TEST(nothrow_callable, functions)
{
    EXPECT_FALSE((P2300_nothrow_callable<void()>));
    EXPECT_TRUE((P2300_nothrow_callable<void() noexcept>));
    EXPECT_FALSE((P2300_nothrow_callable<void() noexcept, int>));
    EXPECT_TRUE((P2300_nothrow_callable<void(td::type<false>) noexcept, td::type<false>>));
    EXPECT_FALSE((P2300_nothrow_callable<void(td::type<false>), td::type<false>>));
    EXPECT_FALSE((P2300_nothrow_callable<void(td::type<true>) noexcept, td::type<true>>));
    EXPECT_TRUE((P2300_nothrow_callable<void(td::type<true>&) noexcept, td::type<true>&>));
    EXPECT_TRUE((P2300_nothrow_callable<td::type<true>() noexcept>));
    EXPECT_TRUE((P2300_nothrow_callable<td::type<true>&() noexcept>));
}

TEST(nothrow_callable, function_pointers)
{
    EXPECT_FALSE((P2300_nothrow_callable<void(*)()>));
    EXPECT_TRUE((P2300_nothrow_callable<void(*)() noexcept>));
    EXPECT_FALSE((P2300_nothrow_callable<void(*)() noexcept, int>));
    EXPECT_TRUE((P2300_nothrow_callable<void(*)(td::type<false>) noexcept, td::type<false>>));
    EXPECT_FALSE((P2300_nothrow_callable<void(*)(td::type<false>), td::type<false>>));
    EXPECT_FALSE((P2300_nothrow_callable<void(*)(td::type<true>) noexcept, td::type<true>>));
    EXPECT_TRUE((P2300_nothrow_callable<void(*)(td::type<true>&) noexcept, td::type<true>&>));
    EXPECT_TRUE((P2300_nothrow_callable<td::type<true>(*)() noexcept>));
    EXPECT_TRUE((P2300_nothrow_callable<td::type<true>&(*)() noexcept>));
}

TEST(nothrow_callable, function_objects)
{
    EXPECT_FALSE((P2300_nothrow_callable<decltype([](){})>));
    EXPECT_TRUE((P2300_nothrow_callable<decltype([]() noexcept {})>));
    EXPECT_FALSE((P2300_nothrow_callable<decltype([]() noexcept {}), int>));
}
