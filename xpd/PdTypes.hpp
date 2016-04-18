/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef Z_PD_TYPES_HPP
#define Z_PD_TYPES_HPP

#include "xpd_tie.hpp"
#include "xpd_symbol.hpp"
#include "xpd_atom.hpp"

#include <iostream>
#include <memory>
#include <cassert>
#include <cstring>
#include <cmath>
#include <exception>

namespace xpd
{
#if (__cplusplus <= 199711L)
#ifdef _WIN32
#include <windows.h>
    class mutex
    {
    public:
        mutex() {InitializeCriticalSection(&m_mutex, NULL);}
        ~mutex() {DeleteCriticalSection(&m_mutex);}
        void lock() {EnterCriticalSection(&m_mutex);}
        void unlock() {LeaveCriticalSection(&m_mutex);}
    private:
        CRITICAL_SECTION m_mutex;
    };
#else
#include <pthread.h>
    class mutex
    {
    public:
        mutex() {pthread_mutex_init(&m_mutex, NULL);}
        ~mutex() {pthread_mutex_destroy(&m_mutex);}
        void lock() {pthread_mutex_lock(&m_mutex);}
        void unlock() {pthread_mutex_unlock(&m_mutex);}
    private:
        pthread_mutex_t m_mutex;
    };
#endif
#else
#include <mutex>
    typedef std::mutex mutex;
#endif

    
    template<typename T> class Point
    {
    public:
        T x;
        T y;
    };
    
    template<typename T> class Rectangle
    {
    public:
        T x;
        T y;
        T w;
        T h;
    };    
    
    //! @brief The smuggler is optimized for internal use.
    //! @details The class doesn't break the efficiency of creation of some type, but you
    //! should use it if and only if you know what you do.
    class smuggler
    {
    public:
        ~smuggler() noexcept {}
    protected:
        inline static constexpr void const* gettie(tie const& tie) noexcept {return tie.ptr;}
        inline static constexpr tie createtie(void *ptr) noexcept {return tie(ptr);}
        inline static constexpr void const* getsymbol(symbol const& symbol) noexcept {return symbol.ptr;}
        inline static constexpr symbol createsymbol(void *ptr) noexcept {return symbol(ptr);}
        //inline static constexpr void const* getvector(vector const& list) noexcept {return list.ptr;}
        //inline static vector createvector(void *ptr) noexcept {return vector(ptr);}
    };
}

#endif // Z_PD_TYPES_HPP
