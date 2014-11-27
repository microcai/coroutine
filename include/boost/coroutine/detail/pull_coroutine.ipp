
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

template< typename T >
pull_coroutine< T >::pull_coroutine( push_coroutine< T > * other) :
    other_( other),
    caller_( other_->callee_),
    callee_( other_->caller_),
    preserve_fpu_( other_->preserve_fpu_),
    state_( 0),
    except_() {
}

template< typename T >
template< typename Fn >
pull_coroutine< T >::pull_coroutine( Fn && fn, bool preserve_fpu) :
    pull_coroutine( fixedsize(), std::forward< Fn >( fn), preserve_fpu) {
}

template< typename T >
template< typename StackAllocator, typename Fn >
pull_coroutine< T >::pull_coroutine( StackAllocator salloc, Fn && fn, bool preserve_fpu) :
    other_( nullptr),
    caller_( boost::context::execution_context::current() ),
    callee_( salloc,
             [=,&fn](){
                try {
                    // create synthesized push_coroutine< T >
                    push_coroutine< T > synthesized( this);
                    other_ = & synthesized;
                    // call coroutine-fn with synthesized pull_coroutine as argument
                    fn( synthesized);
                } catch ( forced_unwind const&) {
                    // do nothing for unwinding exception
                } catch (...) {
                    // store other exceptions in exception-pointer
                    except_ = std::current_exception();
                }
                // set termination flags
                state_ |= static_cast< int >( state_t::complete);
                // jump back to caller
                caller_.jump_to( preserve_fpu_);
                BOOST_ASSERT_MSG( false, "pull_coroutine is complete");
            }),
    preserve_fpu_( preserve_fpu),
    state_( static_cast< int >( state_t::unwind) ),
    except_() {
    callee_.jump_to( preserve_fpu_);
}

template< typename T >
pull_coroutine< T > & 
pull_coroutine< T >::operator()() {
    callee_.jump_to( preserve_fpu_);
    if ( except_) {
        std::rethrow_exception( except_);
    }
    // test early-exit-flag
    if ( 0 != ( ( other_->state_) & static_cast< int >( state_t::early_exit) ) ) {
        throw forced_unwind();
    }
    return * this;
}

/*
template< typename T >
pull_coroutine< T >::pull_coroutine( pull_coroutine && rv) :
    other_( nullptr),
    caller_( std::move( rv.caller_) ),
    callee_( std::move( rv.callee_) ),
    preserve_fpu_( std::move( rv.preserve_fpu_) ),
    state_( static_cast< int >( state_t::complete) ),
    except_() {
    std::swap( other_, rv.other_);
    std::swap( state_, rv.state_);
    std::swap( except_, rv.except_);
    other_->other_ = this;
}

template< typename T >
void
pull_coroutine< T >::swap( pull_coroutine & lv) noexcept {
    std::swap( other_, lv.other_);
    std::swap( caller_, lv.caller_);
    std::swap( callee_, lv.callee_);
    std::swap( preserve_fpu_, lv.preserve_fpu_);
    std::swap( state_, lv.state_);
    std::swap( except_, lv.except_);
    other_->other_ = this;
}
*/

template< typename T >
pull_coroutine< T & >::pull_coroutine( push_coroutine< T & > * other) :
    other_( other),
    caller_( other_->callee_),
    callee_( other_->caller_),
    preserve_fpu_( other_->preserve_fpu_),
    state_( 0),
    except_() {
}

template< typename T >
template< typename Fn >
pull_coroutine< T & >::pull_coroutine( Fn && fn, bool preserve_fpu) :
    pull_coroutine( fixedsize(), std::forward< Fn >( fn), preserve_fpu) {
}

template< typename T >
template< typename StackAllocator, typename Fn >
pull_coroutine< T & >::pull_coroutine( StackAllocator salloc, Fn && fn, bool preserve_fpu) :
    other_( nullptr),
    caller_( boost::context::execution_context::current() ),
    callee_( salloc,
             [=,&fn](){
                try {
                    // create synthesized push_coroutine< T & >
                    push_coroutine< T & > synthesized( this);
                    other_ = & synthesized;
                    // call coroutine-fn with synthesized pull_coroutine as argument
                    fn( synthesized);
                } catch ( forced_unwind const&) {
                    // do nothing for unwinding exception
                } catch (...) {
                    // store other exceptions in exception-pointer
                    except_ = std::current_exception();
                }
                // set termination flags
                state_ |= static_cast< int >( state_t::complete);
                // jump back to caller
                caller_.jump_to( preserve_fpu_);
                BOOST_ASSERT_MSG( false, "pull_coroutine is complete");
            }),
    preserve_fpu_( preserve_fpu),
    state_( static_cast< int >( state_t::unwind) ),
    except_() {
    callee_.jump_to( preserve_fpu_);
}

template< typename T >
pull_coroutine< T & > &
pull_coroutine< T & >::operator()() {
    callee_.jump_to( preserve_fpu_);
    if ( except_) {
        std::rethrow_exception( except_);
    }
    // test early-exit-flag
    if ( 0 != ( ( other_->state_) & static_cast< int >( state_t::early_exit) ) ) {
        throw forced_unwind();
    }
    return * this;
}

/*
template< typename T >
pull_coroutine< T & >::pull_coroutine( pull_coroutine && rv) :
    other_( nullptr),
    caller_( std::move( rv.caller_) ),
    callee_( std::move( rv.callee_) ),
    preserve_fpu_( std::move( rv.preserve_fpu_) ),
    state_( static_cast< int >( state_t::complete) ),
    except_() {
    std::swap( other_, rv.other_);
    std::swap( state_, rv.state_);
    std::swap( except_, rv.except_);
    other_->other_ = this;
}

template< typename T >
void
pull_coroutine< T & >::swap( pull_coroutine & lv) noexcept {
    std::swap( other_, lv.other_);
    std::swap( caller_, lv.caller_);
    std::swap( callee_, lv.callee_);
    std::swap( preserve_fpu_, lv.preserve_fpu_);
    std::swap( state_, lv.state_);
    std::swap( except_, lv.except_);
    other_->other_ = this;
}
*/

pull_coroutine< void >::pull_coroutine( push_coroutine< void > * other) :
    other_( other),
    caller_( other_->callee_),
    callee_( other_->caller_),
    preserve_fpu_( other_->preserve_fpu_),
    state_( 0),
    except_() {
}

template< typename Fn >
pull_coroutine< void >::pull_coroutine( Fn && fn, bool preserve_fpu) :
    pull_coroutine( fixedsize(), std::forward< Fn >( fn), preserve_fpu) {
}

template< typename StackAllocator, typename Fn >
pull_coroutine< void >::pull_coroutine( StackAllocator salloc, Fn && fn, bool preserve_fpu) :
    other_( nullptr),
    caller_( boost::context::execution_context::current() ),
    callee_( salloc,
             [=,&fn](){
                try {
                    // create synthesized push_coroutine< void >
                    push_coroutine< void > synthesized( this);
                    other_ = & synthesized;
                    // call coroutine-fn with synthesized pull_coroutine as argument
                    fn( synthesized);
                } catch ( forced_unwind const&) {
                    // do nothing for unwinding exception
                } catch (...) {
                    // store other exceptions in exception-pointer
                    except_ = std::current_exception();
                }
                // set termination flags
                state_ |= static_cast< int >( state_t::complete);
                // jump back to caller
                caller_.jump_to( preserve_fpu_);
                BOOST_ASSERT_MSG( false, "pull_coroutine is complete");
            }),
    preserve_fpu_( preserve_fpu),
    state_( static_cast< int >( state_t::unwind) ),
    except_() {
    callee_.jump_to( preserve_fpu_);
}

pull_coroutine< void > &
pull_coroutine< void >::operator()() {
    callee_.jump_to( preserve_fpu_);
    if ( except_) {
        std::rethrow_exception( except_);
    }
    // test early-exit-flag
    if ( 0 != ( ( other_->state_) & static_cast< int >( state_t::early_exit) ) ) {
        throw forced_unwind();
    }
    return * this;
}

/*
pull_coroutine< void >::pull_coroutine( pull_coroutine && rv) :
    other_( nullptr),
    caller_( std::move( rv.caller_) ),
    callee_( std::move( rv.callee_) ),
    preserve_fpu_( std::move( rv.preserve_fpu_) ),
    state_( static_cast< int >( state_t::complete) ),
    except_() {
    std::swap( other_, rv.other_);
    std::swap( state_, rv.state_);
    std::swap( except_, rv.except_);
    other_->other_ = this;
}

void
pull_coroutine< void >::swap( pull_coroutine & lv) noexcept {
    std::swap( other_, lv.other_);
    std::swap( caller_, lv.caller_);
    std::swap( callee_, lv.callee_);
    std::swap( preserve_fpu_, lv.preserve_fpu_);
    std::swap( state_, lv.state_);
    std::swap( except_, lv.except_);
    other_->other_ = this;
}
*/

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES_DETAIL_PULL_COROUTINE_IPP
