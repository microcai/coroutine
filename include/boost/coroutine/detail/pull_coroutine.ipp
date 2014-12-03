
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES_DETAIL_PULL_COROUTINE_IPP
#define BOOST_COROUTINES_DETAIL_PULL_COROUTINE_IPP

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

// pull_coroutine< T >

template< typename T >
pull_coroutine< T >::pull_coroutine( control_block * cb) :
    cb_( cb) {
}

template< typename T >
bool
pull_coroutine< T >::has_result_() const {
    return nullptr != cb_->other->t;
}

template< typename T >
template< typename Fn >
pull_coroutine< T >::pull_coroutine( Fn && fn, bool preserve_fpu) :
    pull_coroutine( fixedsize(), std::forward< Fn >( fn), preserve_fpu) {
}

template< typename T >
template< typename StackAllocator, typename Fn >
pull_coroutine< T >::pull_coroutine( StackAllocator salloc, Fn && fn, bool preserve_fpu) :
    cb_( new control_block( salloc, std::forward< Fn >( fn), preserve_fpu) ) {
}

template< typename T >
pull_coroutine< T >::~pull_coroutine() {
}

template< typename T >
pull_coroutine< T > & 
pull_coroutine< T >::operator()() {
    cb_->jump_to();
    return * this;
}

template< typename T >
pull_coroutine< T >::operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

template< typename T >
bool
pull_coroutine< T >::operator!() const noexcept {
    return nullptr == cb_ || ! cb_->valid();
}

template< typename T >
T
pull_coroutine< T >::get() const noexcept {
    return * cb_->other->t;
}


template< typename T >
pull_coroutine< T >::pull_coroutine( pull_coroutine && rv) :
    cb_() {
    swap( rv);
}


// pull_coroutine< T & >

template< typename T >
pull_coroutine< T & >::pull_coroutine( control_block * cb) :
    cb_( cb) {
}

template< typename T >
bool
pull_coroutine< T & >::has_result_() const {
    return nullptr != cb_->other->t;
}

template< typename T >
template< typename Fn >
pull_coroutine< T & >::pull_coroutine( Fn && fn, bool preserve_fpu) :
    pull_coroutine( fixedsize(), std::forward< Fn >( fn), preserve_fpu) {
}

template< typename T >
template< typename StackAllocator, typename Fn >
pull_coroutine< T & >::pull_coroutine( StackAllocator salloc, Fn && fn, bool preserve_fpu) :
    cb_( new control_block( salloc, std::forward< Fn >( fn), preserve_fpu) ) {
}

template< typename T >
pull_coroutine< T & >::~pull_coroutine() {
}

template< typename T >
pull_coroutine< T & > &
pull_coroutine< T & >::operator()() {
    cb_->jump_to();
    return * this;
}

template< typename T >
pull_coroutine< T & >::operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

template< typename T >
bool
pull_coroutine< T & >::operator!() const noexcept {
    return nullptr == cb_ || ! cb_->valid();
}

template< typename T >
T &
pull_coroutine< T & >::get() const noexcept {
    return * cb_->other->t;
}


template< typename T >
pull_coroutine< T & >::pull_coroutine( pull_coroutine && rv) :
    cb_() {
    swap( rv);
}


// pull_coroutine< void >

pull_coroutine< void >::pull_coroutine( control_block * cb) :
    cb_( cb) {
}

template< typename Fn >
pull_coroutine< void >::pull_coroutine( Fn && fn, bool preserve_fpu) :
    pull_coroutine( fixedsize(), std::forward< Fn >( fn), preserve_fpu) {
}

template< typename StackAllocator, typename Fn >
pull_coroutine< void >::pull_coroutine( StackAllocator salloc, Fn && fn, bool preserve_fpu) :
    cb_( new control_block( salloc, std::forward< Fn >( fn), preserve_fpu) ) {
}

pull_coroutine< void >::~pull_coroutine() {
}

pull_coroutine< void > &
pull_coroutine< void >::operator()() {
    cb_->jump_to();
    return * this;
}

pull_coroutine< void >::operator bool() const noexcept {
    return nullptr != cb_ && cb_->valid();
}

bool
pull_coroutine< void >::operator!() const noexcept {
    return nullptr == cb_ || ! cb_->valid();
}


pull_coroutine< void >::pull_coroutine( pull_coroutine && rv) :
    cb_() {
    swap( rv);
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES_DETAIL_PULL_COROUTINE_IPP
