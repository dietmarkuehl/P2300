// include/p2300/tag_invoke.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_INCLUDE_P2300_TAG_INVOKE
#define INCLUDED_INCLUDE_P2300_TAG_INVOKE

// ----------------------------------------------------------------------------
// [func.tag_invoke]
#include <type_traits>

namespace std {
    template <auto& _Tag>
    using tag_t = decay_t<decltype(_Tag)>;
    namespace _Tag_invoke {
        void tag_invoke();

        template <class _Ty>
        concept _Has_class_or_enum_type = is_class_v<remove_reference_t<_Ty>>
            || is_enum_v<remove_reference_t<_Ty>>
            || is_union_v<remove_reference_t<_Ty>>;

        template <class _Tag, class... _Args>
        concept _Tag_invocable_no_adl = _Has_class_or_enum_type<_Tag> && requires {
            {tag_invoke(std::declval<_Tag>(), std::declval<_Args>()...)};
        };

        template <class _Tag, class... _Args>
        concept _Nothrow_tag_invocable_no_adl = _Has_class_or_enum_type<_Tag> && requires {
            {tag_invoke(std::declval<_Tag>(), std::declval<_Args>()...)} noexcept;
        };

        template <class _Tag, class _Arg>
        concept _Tag_observable_no_adl = _Has_class_or_enum_type<_Tag> && requires {
            {tag_invoke(std::declval<_Tag>(), std::declval<add_const_t<_Arg>>())} noexcept;
        };

        class _Cpo {
        public:
            template <class _Tag, class... _Args>
                requires _Tag_invocable_no_adl<_Tag, _Args...>
            constexpr auto operator()(_Tag __tag, _Args&&... __args) const
                noexcept(noexcept(tag_invoke(std::declval<_Tag>(), std::declval<_Args>()...)))
                -> decltype(tag_invoke(std::declval<_Tag>(), std::declval<_Args>()...)) {
                return tag_invoke(std::forward<_Tag>(__tag), std::forward<_Args>(__args)...);
            }
        };
    } // namespace _Tag_invoke

    inline namespace _Cpos {
        inline constexpr _Tag_invoke::_Cpo tag_invoke;
    }

    template <class _Tag, class... _Args>
    concept tag_invocable = _Tag_invoke::_Tag_invocable_no_adl<_Tag, _Args...>;

    template <class _Tag, class... _Args>
    concept nothrow_tag_invocable = _Tag_invoke::_Nothrow_tag_invocable_no_adl<_Tag, _Args...>;

    template <class _Tag, class _Arg>
    concept _Tag_observable = _Tag_invoke::_Tag_observable_no_adl<_Tag, _Arg>;

    template <class _Tag, class... _Args>
    struct tag_invoke_result {};

    template <class _Tag, class... _Args>
        requires _Tag_invoke::_Tag_invocable_no_adl<_Tag, _Args...>
    struct tag_invoke_result<_Tag, _Args...> {
        using type = decltype(tag_invoke(std::declval<_Tag>(), std::declval<_Args>()...));
    };

    template <class _Tag, class... _Args>
    using tag_invoke_result_t = decltype(tag_invoke(std::declval<_Tag>(), std::declval<_Args>()...));
} // namespace std

// ----------------------------------------------------------------------------

#endif
