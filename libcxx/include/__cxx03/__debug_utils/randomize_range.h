//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___CXX03___LIBCXX_DEBUG_RANDOMIZE_RANGE_H
#define _LIBCPP___CXX03___LIBCXX_DEBUG_RANDOMIZE_RANGE_H

#include <__cxx03/__config>

#ifdef _LIBCPP_DEBUG_RANDOMIZE_UNSPECIFIED_STABILITY
#  include <__cxx03/__algorithm/shuffle.h>
#  include <__cxx03/__type_traits/is_constant_evaluated.h>
#endif

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _AlgPolicy, class _Iterator, class _Sentinel>
_LIBCPP_HIDE_FROM_ABI void __debug_randomize_range(_Iterator __first, _Sentinel __last) {
#ifdef _LIBCPP_DEBUG_RANDOMIZE_UNSPECIFIED_STABILITY
#  error Support for unspecified stability is only for C++11 and higher

  if (!__libcpp_is_constant_evaluated())
    std::__shuffle<_AlgPolicy>(__first, __last, __libcpp_debug_randomizer());
#else
  (void)__first;
  (void)__last;
#endif
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___CXX03___LIBCXX_DEBUG_RANDOMIZE_RANGE_H
