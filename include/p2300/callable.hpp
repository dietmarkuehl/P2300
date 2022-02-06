// include/p2300/callable.hpp                                         -*-C++-*-
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

#ifndef _INCLUDED_INCLUDE_P2300_CALLABLE
#define _INCLUDED_INCLUDE_P2300_CALLABLE

#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------
// [function.syn]

namespace std
{
    template <typename _Fn, typename... _Args>
    concept _Callable
        = requires (_Fn&& __fn, _Args&&... __args) {
            std::forward<_Fn>(__fn)(std::forward<_Args>(__args)...);
        }
        ;

    template <typename _Fn, typename... _Args>
    concept _Nothrow_callable
        =  _Callable<_Fn, _Args...>
        && requires (_Fn&& __fn, _Args&&... __args) {
            { std::forward<_Fn>(__fn)(std::forward<_Args>(__args)...) } noexcept;
        }
        ;

    template <typename _Fn, typename... _Args>
    using _Call_result_t = decltype(declval<_Fn>()(declval<_Args>()...));
}

// ----------------------------------------------------------------------------

#endif
