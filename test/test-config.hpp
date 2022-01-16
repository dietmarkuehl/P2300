// test/test-config.hpp                                               -*-C++-*-
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

#ifndef INCLUDED_TEST_TEST_CONFIG
#define INCLUDED_TEST_TEST_CONFIG

// ----------------------------------------------------------------------------
// The top-level namespace for P2300 entities; std for the standard library.

#define P2300_std std

// ----------------------------------------------------------------------------
// Names for the standard library header files.

#define P2300_functional <functional.hpp>
#define P2300_type_traits <type_traits>

// ----------------------------------------------------------------------------
// Fully qualified names for exposition-only entities: use own name or don't
// define to disable corresponding tests.

#define P2300_callable ::std::_Callable
#define P2300_nothrow_callable ::std::_Nothrow_callable
#define P2300_call_result_t ::std::_Call_result_t

// ----------------------------------------------------------------------------

#endif
