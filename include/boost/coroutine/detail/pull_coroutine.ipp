
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
template< typename StackAllocator, typename Fn >
pull_coroutine< T >::control_block::control_block( StackAllocator salloc, Fn && fn_, bool preserve_fpu_) :
    use_count( 0),
    other( nullptr),
    caller( boost::context::execution_context::current() ),
    callee( salloc,
             [=,&fn_](){
                try {
                    // create synthesized push_coroutine< T >
                    typename push_coroutine< T >::control_block synthesized_cb( this);
                    push_coroutine< T > synthesized( & synthesized_cb);
                    other = & synthesized_cb;
                    // call coroutine-fn with synthesized pull_coroutine as argument
                    Fn fn( std::forward< Fn >( fn_) );
                    fn( synthesized);
                } catch ( forced_unwind const&) {
                    // do nothing for unwinding exception
                } catch (...) {
                    // store other exceptions in exception-pointer
                    except = std::current_exception();
                }
                // set termination flags
                state |= static_cast< int >( state_t::complete);
                // jump back to caller
                caller.jump_to( preserve_fpu);
                BOOST_ASSERT_MSG( false, "pull_coroutine is complete");
            }),
    preserve_fpu( preserve_fpu_),
    state( static_cast< int >( state_t::unwind) ),
    except() {
}

template< typename T >
pull_coroutine< T >::control_block::control_block( typename push_coroutine< T >::control_block * cb) :
    use_count( 1),
    other( cb),
    caller( other->callee),
    callee( other->caller),
    preserve_fpu( other->preserve_fpu),
    state( 0),
    except() {
}

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
    cb_->callee.jump_to( cb_->preserve_fpu);
}

template< typename T >
pull_coroutine< T >::~pull_coroutine() {
    if ( nullptr != cb_ &&
         0 == ( cb_->state & static_cast< int >( state_t::complete ) ) &&
         0 != ( cb_->state & static_cast< int >( state_t::unwind) ) ) {
        // set early-exit flag
        cb_->state |= static_cast< int >( state_t::early_exit);
        cb_->callee.jump_to( cb_->preserve_fpu);
    }
}

template< typename T >
pull_coroutine< T > & 
pull_coroutine< T >::operator()() {
    cb_->callee.jump_to( cb_->preserve_fpu);
    if ( cb_->except) {
        std::rethrow_exception( cb_->except);
    }
    // test early-exit-flag
    if ( 0 != ( ( cb_->other->state) & static_cast< int >( state_t::early_exit) ) ) {
        throw forced_unwind();
    }
    return * this;
}

template< typename T >
pull_coroutine< T >::operator bool() const noexcept {
    return nullptr != cb_ && nullptr != cb_->other && nullptr != cb_->other->t && 0 == ( cb_->state & static_cast< int >( state_t::complete) );
}

template< typename T >
bool
pull_coroutine< T >::operator!() const noexcept {
    return nullptr == cb_ || nullptr == cb_->other || nullptr == cb_->other->t || 0 != ( cb_->state & static_cast< int >( state_t::complete) );
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
template< typename StackAllocator, typename Fn >
pull_coroutine< T & >::control_block::control_block( StackAllocator salloc, Fn && fn_, bool preserve_fpu_) :
    use_count( 0),
    other( nullptr),
    caller( boost::context::execution_context::current() ),
    callee( salloc,
             [=,&fn_](){
                try {
                    // create synthesized push_coroutine< T >
                    typename push_coroutine< T & >::control_block synthesized_cb( this);
                    push_coroutine< T & > synthesized( & synthesized_cb);
                    other = & synthesized_cb;
                    // call coroutine-fn with synthesized pull_coroutine as argument
                    Fn fn( std::forward< Fn >( fn_) );
                    fn( synthesized);
                } catch ( forced_unwind const&) {
                    // do nothing for unwinding exception
                } catch (...) {
                    // store other exceptions in exception-pointer
                    except = std::current_exception();
                }
                // set termination flags
                state |= static_cast< int >( state_t::complete);
                // jump back to caller
                caller.jump_to( preserve_fpu);
                BOOST_ASSERT_MSG( false, "pull_coroutine is complete");
            }),
    preserve_fpu( preserve_fpu_),
    state( static_cast< int >( state_t::unwind) ),
    except() {
}

template< typename T >
pull_coroutine< T & >::control_block::control_block( typename push_coroutine< T & >::control_block * cb) :
    use_count( 1),
    other( cb),
    caller( other->callee),
    callee( other->caller),
    preserve_fpu( other->preserve_fpu),
    state( 0),
    except() {
}

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
    cb_->callee.jump_to( cb_->preserve_fpu);
}

template< typename T >
pull_coroutine< T & >::~pull_coroutine() {
    if ( nullptr != cb_ &&
         0 == ( cb_->state & static_cast< int >( state_t::complete ) ) &&
         0 != ( cb_->state & static_cast< int >( state_t::unwind) ) ) {
        // set early-exit flag
        cb_->state |= static_cast< int >( state_t::early_exit);
        cb_->callee.jump_to( cb_->preserve_fpu);
    }
}

template< typename T >
pull_coroutine< T & > &
pull_coroutine< T & >::operator()() {
    cb_->callee.jump_to( cb_->preserve_fpu);
    if ( cb_->except) {
        std::rethrow_exception( cb_->except);
    }
    // test early-exit-flag
    if ( 0 != ( ( cb_->other->state) & static_cast< int >( state_t::early_exit) ) ) {
        throw forced_unwind();
    }
    return * this;
}

template< typename T >
pull_coroutine< T & >::operator bool() const noexcept {
    return nullptr != cb_ && nullptr != cb_->other && nullptr != cb_->other->t && 0 == ( cb_->state & static_cast< int >( state_t::complete) );
}

template< typename T >
bool
pull_coroutine< T & >::operator!() const noexcept {
    return nullptr == cb_ || nullptr == cb_->other || nullptr == cb_->other->t || 0 != ( cb_->state & static_cast< int >( state_t::complete) );
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

template< typename StackAllocator, typename Fn >
pull_coroutine< void >::control_block::control_block( StackAllocator salloc, Fn && fn_, bool preserve_fpu_) :
    use_count( 0),
    other( nullptr),
    caller( boost::context::execution_context::current() ),
    callee( salloc,
             [=,&fn_](){
                try {
                    // create synthesized push_coroutine< T >
                    typename push_coroutine< void >::control_block synthesized_cb( this);
                    push_coroutine< void > synthesized( & synthesized_cb);
                    other = & synthesized_cb;
                    // call coroutine-fn with synthesized pull_coroutine as argument
                    Fn fn( std::forward< Fn >( fn_) );
                    fn( synthesized);
                } catch ( forced_unwind const&) {
                    // do nothing for unwinding exception
                } catch (...) {
                    // store other exceptions in exception-pointer
                    except = std::current_exception();
                }
                // set termination flags
                state |= static_cast< int >( state_t::complete);
                // jump back to caller
                caller.jump_to( preserve_fpu);
                BOOST_ASSERT_MSG( false, "pull_coroutine is complete");
            }),
    preserve_fpu( preserve_fpu_),
    state( static_cast< int >( state_t::unwind) ),
    except() {
}

pull_coroutine< void >::control_block::control_block( typename push_coroutine< void >::control_block * cb) :
    use_count( 1),
    other( cb),
    caller( other->callee),
    callee( other->caller),
    preserve_fpu( other->preserve_fpu),
    state( 0),
    except() {
}

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
    cb_->callee.jump_to( cb_->preserve_fpu);
}

pull_coroutine< void >::~pull_coroutine() {
    if ( nullptr != cb_ &&
         0 == ( cb_->state & static_cast< int >( state_t::complete ) ) &&
         0 != ( cb_->state & static_cast< int >( state_t::unwind) ) ) {
        // set early-exit flag
        cb_->state |= static_cast< int >( state_t::early_exit);
        cb_->callee.jump_to( cb_->preserve_fpu);
    }
}

pull_coroutine< void > &
pull_coroutine< void >::operator()() {
    cb_->callee.jump_to( cb_->preserve_fpu);
    if ( cb_->except) {
        std::rethrow_exception( cb_->except);
    }
    // test early-exit-flag
    if ( 0 != ( ( cb_->other->state) & static_cast< int >( state_t::early_exit) ) ) {
        throw forced_unwind();
    }
    return * this;
}

pull_coroutine< void >::operator bool() const noexcept {
    return nullptr != cb_ && nullptr != cb_->other && 0 == ( cb_->state & static_cast< int >( state_t::complete) );
}

bool
pull_coroutine< void >::operator!() const noexcept {
    return nullptr == cb_ || nullptr == cb_->other || 0 != ( cb_->state & static_cast< int >( state_t::complete) );
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
