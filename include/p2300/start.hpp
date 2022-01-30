// include/p2300/start.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_INCLUDE_P2300_START
#define INCLUDED_INCLUDE_P2300_START

#include <functional.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace std {
    namespace _Start {
        class _Cpo {
        public:
            template <class _OperationState>
                requires nothrow_tag_invocable<_Cpo, _OperationState>
                    && std::is_lvalue_reference_v<_OperationState>
            auto operator()(_OperationState&& __state) const noexcept -> void
            {
                tag_invoke(*this, __state);
            }
        };
    }

    namespace execution {
        inline namespace _Cpos {
            using start_t = _Start::_Cpo;
            inline constexpr start_t start{};
        }
    }
}

// ----------------------------------------------------------------------------

#endif
