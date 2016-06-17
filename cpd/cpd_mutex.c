/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.


#include "cpd_mutex.h"

#ifdef _WIN32

int cpd_mutex_init(cpd_mutex* mutex)
{
    InitializeCriticalSection(mutex);
    return 0;
}

int cpd_mutex_lock(cpd_mutex* mutex)
{
    EnterCriticalSection(mutex);
    return 0;
}

int cpd_mutex_unlock(cpd_mutex* mutex)
{
    LeaveCriticalSection(mutex);
    return 0;
}

int cpd_mutex_destroy(cpd_mutex* mutex)
{
    DeleteCriticalSection(mutex);
    return 0;
}

#else

int cpd_mutex_init(cpd_mutex* mutex)
{
    return pthread_mutex_init(mutex, NULL);
}

int cpd_mutex_lock(cpd_mutex* mutex)
{
    return pthread_mutex_lock(mutex);
}

int cpd_mutex_unlock(cpd_mutex* mutex)
{
    return pthread_mutex_unlock(mutex);
}

int cpd_mutex_destroy(cpd_mutex* mutex)
{
    return pthread_mutex_destroy(mutex);
}

#endif


