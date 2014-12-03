
//          Copyright Oliver Kowalke 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES_DETAIL_PULL_CONTROL_BLOCK_HPP
#define BOOST_COROUTINES_DETAIL_PULL_CONTROL_BLOCK_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/intrusive_ptr.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace coroutines {
namespace detail {

template< typename T >
struct pull_coroutine< T >::control_block {
    std::size_t                                     use_count;
    typename push_coroutine< T >::control_block *   other;
    boost::context::execution_context               caller;
    boost::context::execution_context               callee;
    bool                                            preserve_fpu;
    int                                             state;
    std::exception_ptr                              except;

    template< typename StackAllocator, typename Fn >
    control_block( StackAllocator, Fn &&, bool);

    explicit control_block( typename push_coroutine< T >::control_block *);

    ~control_block();

    control_block( control_block &) = delete;
    control_block & operator=( control_block &) = delete;

    void jump_to();

    bool valid() const noexcept;

    friend void intrusive_ptr_add_ref( control_block * cb) {
        ++cb->use_count;
    }

    friend void intrusive_ptr_release( control_block * cb) {
        if ( 0 == --cb->use_count) {
            delete cb;
        }
    }
};

template< typename T >
struct pull_coroutine< T & >::control_block {
    std::size_t                                         use_count;
    typename push_coroutine< T & >::control_block   *   other;
    boost::context::execution_context                   caller;
    boost::context::execution_context                   callee;
    bool                                                preserve_fpu;
    int                                                 state;
    std::exception_ptr                                  except;

    template< typename StackAllocator, typename Fn >
    control_block( StackAllocator, Fn &&, bool);

    explicit control_block( typename push_coroutine< T & >::control_block *);

    ~control_block();

    control_block( control_block &) = delete;
    control_block & operator=( control_block &) = delete;

    void jump_to();

    bool valid() const noexcept;

    friend void intrusive_ptr_add_ref( control_block * cb) {
        ++cb->use_count;
    }

    friend void intrusive_ptr_release( control_block * cb) {
        if ( 0 == --cb->use_count) {
            delete cb;
        }
    }
};

struct pull_coroutine< void >::control_block {
    std::size_t                                         use_count;
    typename push_coroutine< void >::control_block  *   other;
    boost::context::execution_context                   caller;
    boost::context::execution_context                   callee;
    bool                                                preserve_fpu;
    int                                                 state;
    std::exception_ptr                                  except;

    template< typename StackAllocator, typename Fn >
    control_block( StackAllocator, Fn &&, bool);

    explicit control_block( typename push_coroutine< void >::control_block *);

    ~control_block();

    control_block( control_block &) = delete;
    control_block & operator=( control_block &) = delete;

    void jump_to();

    bool valid() const noexcept;

    friend void intrusive_ptr_add_ref( control_block * cb) {
        ++cb->use_count;
    }

    friend void intrusive_ptr_release( control_block * cb) {
        if ( 0 == --cb->use_count) {
            delete cb;
        }
    }
};

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES_DETAIL_PULL_CONTROL_BLOCK_HPP
