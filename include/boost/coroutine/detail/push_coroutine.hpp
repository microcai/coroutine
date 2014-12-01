
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES_DETAIL_PUSH_COROUTINE_HPP
#define BOOST_COROUTINES_DETAIL_PUSH_COROUTINE_HPP

#include <exception>
#include <iterator>
#include <type_traits>
#include <utility>

#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/context/execution_context.hpp>
#include <boost/intrusive_ptr.hpp>

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
class push_coroutine {
private:
    template< typename X >
    friend class pull_coroutine;

    struct control_block;

    boost::intrusive_ptr< control_block >            cb_;

    explicit push_coroutine( control_block *);

public:
    template< typename Fn >
    push_coroutine( Fn &&, bool = false);

    template< typename StackAllocator, typename Fn >
    push_coroutine( StackAllocator, Fn &&, bool = false);

    ~push_coroutine();

    push_coroutine( push_coroutine const&) = delete;
    push_coroutine & operator=( push_coroutine const&) = delete;

    push_coroutine( push_coroutine &&);

    push_coroutine & operator=( push_coroutine && rv) {
        if ( this == & rv) return * this;
        push_coroutine tmp( std::move( rv) );
        swap( tmp);
        return * this;
    }

    push_coroutine & operator()( T);

    explicit operator bool() const noexcept;

    bool operator!() const noexcept;

    void swap( push_coroutine & other) noexcept {
        cb_.swap( other.cb_);
    }

    class iterator : public std::iterator< std::output_iterator_tag, void, void, void, void > {
    private:
        push_coroutine< T > *   c_;

    public:
        iterator() :
            c_( nullptr) {
        }

        explicit iterator( push_coroutine< T > * c) :
            c_( c) {
        }

        iterator & operator=( T t) {
            BOOST_ASSERT( c_);
            if ( ! ( * c_)( t) ) c_ = 0;
            return * this;
        }

        bool operator==( iterator const& other) const {
            return other.c_ == c_;
        }

        bool operator!=( iterator const& other) const {
            return other.c_ != c_;
        }

        iterator & operator*() {
            return * this;
        }

        iterator & operator++() {
            return * this;
        }
    };
};

template< typename T >
class push_coroutine< T & > {
private:
    template< typename X >
    friend class pull_coroutine;

    struct control_block;

    boost::intrusive_ptr< control_block >            cb_;

    explicit push_coroutine( control_block *);

public:
    template< typename Fn >
    push_coroutine( Fn &&, bool = false);

    template< typename StackAllocator, typename Fn >
    push_coroutine( StackAllocator, Fn &&, bool = false);

    ~push_coroutine();

    push_coroutine( push_coroutine const&) = delete;
    push_coroutine & operator=( push_coroutine const&) = delete;

    push_coroutine( push_coroutine &&);

    push_coroutine & operator=( push_coroutine && rv) {
        if ( this == & rv) return * this;
        push_coroutine tmp( std::move( rv) );
        swap( tmp);
        return * this;
    }

    push_coroutine & operator()( T &);

    explicit operator bool() const noexcept;

    bool operator!() const noexcept;

    void swap( push_coroutine & other) noexcept {
        cb_.swap( other.cb_);
    }

    class iterator : public std::iterator< std::output_iterator_tag, void, void, void, void > {
    private:
        push_coroutine< T & >   *   c_;

    public:
        iterator() :
            c_( nullptr) {
        }

        explicit iterator( push_coroutine< T & > * c) :
            c_( c) {
        }

        iterator & operator=( T & t) {
            BOOST_ASSERT( c_);
            if ( ! ( * c_)( t) ) c_ = 0;
            return * this;
        }

        bool operator==( iterator const& other) const {
            return other.c_ == c_;
        }

        bool operator!=( iterator const& other) const {
            return other.c_ != c_;
        }

        iterator & operator*() {
            return * this;
        }

        iterator & operator++() {
            return * this;
        }
    };
};

template<>
class push_coroutine< void > {
private:
    template< typename X >
    friend class pull_coroutine;

    struct control_block;

    boost::intrusive_ptr< control_block >            cb_;

    explicit push_coroutine( control_block *);

public:
    template< typename Fn >
    push_coroutine( Fn &&, bool = false);

    template< typename StackAllocator, typename Fn >
    push_coroutine( StackAllocator, Fn &&, bool = false);

    ~push_coroutine();

    push_coroutine( push_coroutine const&) = delete;
    push_coroutine & operator=( push_coroutine const&) = delete;

    push_coroutine( push_coroutine &&);

    push_coroutine & operator=( push_coroutine && rv) {
        if ( this == & rv) return * this;
        push_coroutine tmp( std::move( rv) );
        swap( tmp);
        return * this;
    }

    push_coroutine & operator()();

    explicit operator bool() const noexcept;

    bool operator!() const noexcept;

    void swap( push_coroutine & other) noexcept {
        cb_.swap( other.cb_);
    }
};

template< typename T >
typename push_coroutine< T >::iterator
begin( push_coroutine< T > & c) {
    return typename push_coroutine< T >::iterator( & c);
}

template< typename T >
typename push_coroutine< T >::iterator
end( push_coroutine< T > &) {
    return typename push_coroutine< T >::iterator();
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES_DETAIL_PUSH_COROUTINE_HPP
