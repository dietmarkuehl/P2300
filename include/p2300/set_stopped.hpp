// include/p2300/set_stopped.hpp                                      -*-C++-*-
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

#ifndef INCLUDED_INCLUDE_P2300_SET_STOPPED
#define INCLUDED_INCLUDE_P2300_SET_STOPPED

#include <functional>
#include <utility>

// ----------------------------------------------------------------------------
// [exec.set_stopped]

namespace std {
    namespace _Set_stopped {
        class _Cpo {
        public:
            template <class _Receiver>
                requires nothrow_tag_invocable<_Cpo, _Receiver>
            auto operator()(_Receiver&& __receiver) const noexcept -> void
            {
                std::tag_invoke(*this, std::forward<_Receiver>(__receiver));
            }
        };
    }

    namespace execution {
        using set_stopped_t = _Set_stopped::_Cpo;
        inline namespace _Cpos {
            inline constexpr set_stopped_t set_stopped{};
        }
    }
}

// ----------------------------------------------------------------------------

#endif
