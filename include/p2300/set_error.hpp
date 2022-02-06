// include/p2300/set_error.hpp                                        -*-C++-*-
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

#ifndef INCLUDED_INCLUDE_P2300_SET_ERROR
#define INCLUDED_INCLUDE_P2300_SET_ERROR

#include <functional>
#include <utility>

// ----------------------------------------------------------------------------

namespace std {
    namespace _Set_error {
        class _Cpo {
        public:
            template <class _Receiver, class _Error>
                requires nothrow_tag_invocable<_Cpo, _Receiver, _Error>
            auto operator()(_Receiver&& __receiver, _Error&& __error) const noexcept -> void
            {
                std::tag_invoke(*this, std::forward<_Receiver>(__receiver), std::forward<_Error>(__error));
            }
        };
    }

    namespace execution {
        using set_error_t = _Set_error::_Cpo;
        inline namespace _Cpos {
            inline constexpr set_error_t set_error{};
        }
    }
}

// ----------------------------------------------------------------------------

#endif
