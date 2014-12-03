
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES_DETAIL_PUSH_COROUTINE_IPP
#define BOOST_COROUTINES_DETAIL_PUSH_COROUTINE_IPP

#include <exception>

#include <boost/assert.hpp>
#include <boost/config.hpp>

#include <boost/context/execution_context.hpp>

#include <boost/coroutine/detail/config.hpp>
#include <boost/coroutine/detail/forced_unwind.hpp>
#include <boost/coroutine/detail/state.hpp>
#include <boost/coroutine/fixedsize.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace coroutines {
namespace detail {

// push_coroutine< T >

template< typename T >
push_coroutine< T >::push_coroutine( control_block * cb) :
    cb_( cb) {
}

template< typename T >
template< typename Fn >
push_coroutine< T >::push_coroutine( Fn && fn, bool preserve_fpu) :
    push_coroutine( fixedsize(), std::forward< Fn >( fn), preserve_fpu) {
}

template< typename T >
template< typename StackAllocator, typename Fn >
push_coroutine< T >::push_coroutine( StackAllocator salloc, Fn && fn, bool preserve_fpu) :
    cb_( new control_block( salloc, std::forward< Fn >( fn), preserve_fpu) ) {
}

template< typename T >
push_coroutine< T >::~push_coroutine() {
}

template< typename T >
push_coroutine< T > &
push_coroutine< T >::operator()( T t) {
    cb_->jump_to( t);
    return * this;
}

template< typename T >
push_coroutine< T >::operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

template< typename T >
bool
push_coroutine< T >::operator!() const noexcept {
    return nullptr == cb_ || ! cb_->valid();
}

template< typename T >
push_coroutine< T >::push_coroutine( push_coroutine && rv) :
    cb_() {
    swap( rv);
}


// push_coroutine< T & >

template< typename T >
push_coroutine< T & >::push_coroutine( control_block * cb) :
    cb_( cb) {
}

template< typename T >
template< typename Fn >
push_coroutine< T & >::push_coroutine( Fn && fn, bool preserve_fpu) :
    push_coroutine( fixedsize(), std::forward< Fn >( fn), preserve_fpu) {
}

template< typename T >
template< typename StackAllocator, typename Fn >
push_coroutine< T & >::push_coroutine( StackAllocator salloc, Fn && fn, bool preserve_fpu) :
    cb_( new control_block( salloc, std::forward< Fn >( fn), preserve_fpu) ) {
}

template< typename T >
push_coroutine< T & >::~push_coroutine() {
}

template< typename T >
push_coroutine< T & > &
push_coroutine< T & >::operator()( T & t) {
    cb_->jump_to( t);
    return * this;
}

template< typename T >
push_coroutine< T & >::operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

template< typename T >
bool
push_coroutine< T & >::operator!() const noexcept {
    return nullptr == cb_ || ! cb_->valid();
}

template< typename T >
push_coroutine< T & >::push_coroutine( push_coroutine && rv) :
    cb_() {
    swap( rv);
}


// push_coroutine< void >

push_coroutine< void >::push_coroutine( control_block * cb) :
    cb_( cb) {
}

template< typename Fn >
push_coroutine< void >::push_coroutine( Fn && fn, bool preserve_fpu) :
    push_coroutine( fixedsize(), std::forward< Fn >( fn), preserve_fpu) {
}

template< typename StackAllocator, typename Fn >
push_coroutine< void >::push_coroutine( StackAllocator salloc, Fn && fn, bool preserve_fpu) :
    cb_( new control_block( salloc, std::forward< Fn >( fn), preserve_fpu) ) {
}

push_coroutine< void >::~push_coroutine() {
}

push_coroutine< void > &
push_coroutine< void >::operator()() {
    cb_->jump_to();
    return * this;
}

push_coroutine< void >::operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

bool
push_coroutine< void >::operator!() const noexcept {
    return nullptr == cb_ || ! cb_->valid();
}

push_coroutine< void >::push_coroutine( push_coroutine && rv) :
    cb_() {
    swap( rv);
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES_DETAIL_PUSH_COROUTINE_IPP
