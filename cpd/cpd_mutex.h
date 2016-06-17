/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_mutex_h
#define cpd_mutex_h

#include "cpd_def.h"

//! @defgroup mutex mutex
//! @brief The mutex part of cpd.
//! @details This part manages a mutex.

//! @addtogroup mutex
//! @{

//! @brief The mutex.
#ifdef _WIN32
#include <windows.h>
typedef CRITICAL_SECTION cpd_mutex;
#else
#include <pthread.h>
typedef pthread_mutex_t cpd_mutex;
#endif


//! @brief Initializes a mutex.
CPD_EXTERN int cpd_mutex_init(cpd_mutex* mutex);

//! @brief Locks a mutex.
CPD_EXTERN int cpd_mutex_lock(cpd_mutex* mutex);

//! @brief Unlocks a mutex.
CPD_EXTERN int cpd_mutex_unlock(cpd_mutex* mutex);

//! @brief Destroy a mutex.
CPD_EXTERN int cpd_mutex_destroy(cpd_mutex* mutex);

//! @}


#endif // cpd_mutex_h
