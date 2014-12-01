
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES_DETAIL_PULL_COROUTINE_HPP
#define BOOST_COROUTINES_DETAIL_PULL_COROUTINE_HPP

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
class pull_coroutine {
private:
    template< typename X >
    friend class push_coroutine;

    struct control_block;

    boost::intrusive_ptr< control_block >    cb_;

    explicit pull_coroutine( control_block *);

    bool has_result_() const;

public:
    template< typename Fn >
    pull_coroutine( Fn &&, bool = false);

    template< typename StackAllocator, typename Fn >
    pull_coroutine( StackAllocator, Fn &&, bool = false);

    ~pull_coroutine();

    pull_coroutine( pull_coroutine const&) = delete;
    pull_coroutine & operator=( pull_coroutine const&) = delete;

    pull_coroutine( pull_coroutine &&);

    pull_coroutine & operator=( pull_coroutine && rv) {
        if ( this == & rv) return * this;
        pull_coroutine tmp( std::move( rv) );
        swap( tmp);
        return * this;
    }

    pull_coroutine & operator()();

    explicit operator bool() const noexcept;

    bool operator!() const noexcept;

    T get() const noexcept;

    void swap( pull_coroutine & other) noexcept {
        cb_.swap( other.cb_);
    }

    class iterator : public std::iterator< std::input_iterator_tag, typename std::remove_reference< T >::type > {
    private:
        pull_coroutine< T > *   c_;

        void fetch_() {
            BOOST_ASSERT( nullptr != c_);
            if ( ! ( * c_) ) {
                c_ = nullptr;
                return;
            }
        }

        void increment_() {
            BOOST_ASSERT( nullptr != c_);
            BOOST_ASSERT( * c_);
            ( * c_)();
            fetch_();
        }

    public:
        typedef typename iterator::pointer pointer_t;
        typedef typename iterator::reference reference_t;

        iterator() :
            c_( nullptr) {
        }

        explicit iterator( pull_coroutine< T > * c) :
            c_( c) {
            fetch_();
        }

        iterator( iterator const& other) :
            c_( other.c_) {
        }

        iterator & operator=( iterator const& other) {
            if ( this == & other) return * this;
            c_ = other.c_;
            return * this;
        }

        bool operator==( iterator const& other) const {
            return other.c_ == c_;
        }

        bool operator!=( iterator const& other) const {
            return other.c_ != c_;
        }

        iterator & operator++() {
            increment_();
            return * this;
        }

        iterator operator++( int) {
            iterator tmp( * this);
            ++*this;
            return tmp;
        }

        reference_t operator*() const {
            return * c_->cb_->other->t;
        }

        pointer_t operator->() const {
            return c_->cb_->other->t;
        }
    };

    friend class iterator;
};

template< typename T >
class pull_coroutine< T & > {
private:
    template< typename X >
    friend class push_coroutine;

    struct control_block;

    boost::intrusive_ptr< control_block >            cb_;

    explicit pull_coroutine( control_block *);

    bool has_result_() const;

public:
    template< typename Fn >
    pull_coroutine( Fn &&, bool = false);

    template< typename StackAllocator, typename Fn >
    pull_coroutine( StackAllocator, Fn &&, bool = false);

    ~pull_coroutine();

    pull_coroutine( pull_coroutine const&) = delete;
    pull_coroutine & operator=( pull_coroutine const&) = delete;

    pull_coroutine( pull_coroutine &&);

    pull_coroutine & operator=( pull_coroutine && rv) {
        if ( this == & rv) return * this;
        pull_coroutine tmp( std::move( rv) );
        swap( tmp);
        return * this;
    }

    pull_coroutine & operator()();

    explicit operator bool() const noexcept;

    bool operator!() const noexcept;

    T & get() const noexcept;

    void swap( pull_coroutine & other) noexcept {
        cb_.swap( other.cb_);
    }

    class iterator : public std::iterator< std::input_iterator_tag, typename std::remove_reference< T >::type > {
    private:
        pull_coroutine< T & > *   c_;

        void fetch_() {
            BOOST_ASSERT( nullptr != c_);
            if ( ! ( * c_) ) {
                c_ = nullptr;
                return;
            }
        }

        void increment_() {
            BOOST_ASSERT( nullptr != c_);
            BOOST_ASSERT( * c_);
            ( * c_)();
            fetch_();
        }

    public:
        typedef typename iterator::pointer pointer_t;
        typedef typename iterator::reference reference_t;

        iterator() :
            c_( nullptr) {
        }

        explicit iterator( pull_coroutine< T & > * c) :
            c_( c) {
            fetch_();
        }

        iterator( iterator const& other) :
            c_( other.c_) {
        }

        iterator & operator=( iterator const& other) {
            if ( this == & other) return * this;
            c_ = other.c_;
            return * this;
        }

        bool operator==( iterator const& other) const {
            return other.c_ == c_;
        }

        bool operator!=( iterator const& other) const {
            return other.c_ != c_;
        }

        iterator & operator++() {
            increment_();
            return * this;
        }

        iterator operator++( int) {
            iterator tmp( * this);
            ++*this;
            return tmp;
        }

        reference_t operator*() const {
            return * c_->cb_->other->t;
        }

        pointer_t operator->() const {
            return c_->cb_->other->t;
        }
    };

    friend class iterator;
};

template<>
class pull_coroutine< void > {
private:
    template< typename X >
    friend class push_coroutine;

    struct control_block;

    boost::intrusive_ptr< control_block >            cb_;

    explicit pull_coroutine( control_block *);

public:
    template< typename Fn >
    pull_coroutine( Fn &&, bool = false);

    template< typename StackAllocator, typename Fn >
    pull_coroutine( StackAllocator, Fn &&, bool = false);

    ~pull_coroutine();

    pull_coroutine( pull_coroutine const&) = delete;
    pull_coroutine & operator=( pull_coroutine const&) = delete;

    pull_coroutine( pull_coroutine &&);

    pull_coroutine & operator=( pull_coroutine && rv) {
        if ( this == & rv) return * this;
        pull_coroutine tmp( std::move( rv) );
        swap( tmp);
        return * this;
    }

    pull_coroutine & operator()();

    explicit operator bool() const noexcept;

    bool operator!() const noexcept;

    void swap( pull_coroutine & other) noexcept {
        cb_.swap( other.cb_);
    }
};

template< typename T >
typename pull_coroutine< T >::iterator
begin( pull_coroutine< T > & c) {
    return typename pull_coroutine< T >::iterator( & c);
}

template< typename T >
typename pull_coroutine< T >::iterator
end( pull_coroutine< T > &) {
    return typename pull_coroutine< T >::iterator();
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES_DETAIL_PULL_COROUTINE_HPP
