
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES_DETAIL_PULL_CONTROL_BLOCK_IPP
#define BOOST_COROUTINES_DETAIL_PULL_CONTROL_BLOCK_IPP

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
    callee.jump_to( preserve_fpu);
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
pull_coroutine< T >::control_block::~control_block() {
    if ( 0 == ( state & static_cast< int >( state_t::complete ) ) &&
         0 != ( state & static_cast< int >( state_t::unwind) ) ) {
        // set early-exit flag
        state |= static_cast< int >( state_t::early_exit);
        callee.jump_to( preserve_fpu);
    }
}

template< typename T >
void
pull_coroutine< T >::control_block::jump_to() {
    callee.jump_to( preserve_fpu);
    if ( except) {
        std::rethrow_exception( except);
    }
    // test early-exit-flag
    if ( 0 != ( ( other->state) & static_cast< int >( state_t::early_exit) ) ) {
        throw forced_unwind();
    }
}

template< typename T >
bool
pull_coroutine< T >::control_block::valid() const noexcept {
    return nullptr != other && nullptr != other->t && 0 == ( state & static_cast< int >( state_t::complete) );
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
    callee.jump_to( preserve_fpu);
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
pull_coroutine< T & >::control_block::~control_block() {
    if ( 0 == ( state & static_cast< int >( state_t::complete ) ) &&
         0 != ( state & static_cast< int >( state_t::unwind) ) ) {
        // set early-exit flag
        state |= static_cast< int >( state_t::early_exit);
        callee.jump_to( preserve_fpu);
    }
}

template< typename T >
void
pull_coroutine< T & >::control_block::jump_to() {
    callee.jump_to( preserve_fpu);
    if ( except) {
        std::rethrow_exception( except);
    }
    // test early-exit-flag
    if ( 0 != ( ( other->state) & static_cast< int >( state_t::early_exit) ) ) {
        throw forced_unwind();
    }
}

template< typename T >
bool
pull_coroutine< T & >::control_block::valid() const noexcept {
    return nullptr != other && nullptr != other->t && 0 == ( state & static_cast< int >( state_t::complete) );
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
    callee.jump_to( preserve_fpu);
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

pull_coroutine< void >::control_block::~control_block() {
    if ( 0 == ( state & static_cast< int >( state_t::complete ) ) &&
         0 != ( state & static_cast< int >( state_t::unwind) ) ) {
        // set early-exit flag
        state |= static_cast< int >( state_t::early_exit);
        callee.jump_to( preserve_fpu);
    }
}

void
pull_coroutine< void >::control_block::jump_to() {
    callee.jump_to( preserve_fpu);
    if ( except) {
        std::rethrow_exception( except);
    }
    // test early-exit-flag
    if ( 0 != ( ( other->state) & static_cast< int >( state_t::early_exit) ) ) {
        throw forced_unwind();
    }
}

bool
pull_coroutine< void >::control_block::valid() const noexcept {
    return nullptr != other && 0 == ( state & static_cast< int >( state_t::complete) );
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES_DETAIL_PULL_CONTROL_BLOCK_IPP
