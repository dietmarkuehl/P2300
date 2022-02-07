// test/call_result_t.cpp                                             -*-C++-*-
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
#include P2300_type_traits
#include <gtest/gtest.h>

// ----------------------------------------------------------------------------

TEST(call_result_t, functions)
{
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<void()>, void>));
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<int()>, int>));
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<int&()>, int&>));
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<int&&()>, int&&>));
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<void(bool, double), bool, double>, void>));
}

TEST(call_result_t, function_pointers)
{
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<void(*)()>, void>));
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<int(*)()>, int>));
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<int&(*)()>, int&>));
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<int&&(*)()>, int&&>));
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<void(*)(bool, double), bool, double>, void>));
}

TEST(call_result_t, function_objects)
{
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<decltype([](){})>, void>));
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<decltype([](){ return 0; })>, int>));
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<decltype([r=0]() mutable -> int& { return r; })>, int&>));
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<decltype([](bool, double){}), bool, double>, void>));
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<decltype([](auto v){ return v; }), bool>, bool>));
    EXPECT_TRUE((P2300_std::is_same_v<P2300_call_result_t<decltype([](auto v){ return v; }), double>, double>));
}
