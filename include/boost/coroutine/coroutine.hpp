
//          Copyright Oliver Kowalke 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES_COROUTINE_H
#define BOOST_COROUTINES_COROUTINE_H

#include <exception>

#include <boost/assert.hpp>
#include <boost/config.hpp>

#include <boost/coroutine/detail/coroutine.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace coroutines {

template< typename T >
struct coroutine {
    typedef detail::pull_coroutine< T >     pull_type;
    typedef detail::push_coroutine< T >     push_type;
};

}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES_COROUTINE_H
