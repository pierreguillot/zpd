/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_MUTEX_HPP
#define XPD_MUTEX_HPP

#include "xpd_def.hpp"

#ifdef _XPD_CPP11_NOSUPPORT_
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif
#else
#include <mutex>
#endif

namespace xpd
{
#ifdef _XPD_CPP11_NOSUPPORT_
#ifdef _WIN32
    class mutex
    {
    public:
        mutex() {InitializeCriticalSection(&m_mutex);}
        ~mutex() {DeleteCriticalSection(&m_mutex);}
        void lock() {EnterCriticalSection(&m_mutex);}
        void unlock() {LeaveCriticalSection(&m_mutex);}
    private:
        CRITICAL_SECTION m_mutex;
    };
#else
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
    typedef std::mutex mutex;
#endif
}

#endif // XPD_MUTEX_HPP
