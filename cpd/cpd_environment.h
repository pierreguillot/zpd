/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_environment_h
#define cpd_environment_h

#include "cpd_types.h"

//! @defgroup env environment
//! @brief The global environment of cpd.

//! @addtogroup env
//! @{

//! @brief Initializes the cpd environment.
//! @details The method should be called only one time at the initialization, before
//! everything else.
CPD_EXTERN void cpd_init();

//! @brief Clears the cpd environment.
//! @details The method should be called only one time when the Pure Data environment will
//! never be called again.
CPD_EXTERN void cpd_clear();




//! @brief Gets the major version of Pure Data.
CPD_EXTERN unsigned int cpd_version_getmajor();

//! @brief Gets the minor version of Pure Data.
CPD_EXTERN unsigned int cpd_version_getminor();

//! @brief Gets the bug version of Pure Data.
CPD_EXTERN unsigned int cpd_version_getbug();




//! @brief Clears the search path of Pure Data.
//! @details Clears the search path for all the environment. The path will be used when you
//! try to open a patch, to load an abstraction or an external.
CPD_EXTERN void cpd_searchpath_clear();

//! @brief Adds a path to the search path of Pure Data.
//! @details Adds a path to the search path for all the environment. The path will be used
//! when you try to open a patch, to load an abstraction or an external.
//! @param path The path to add.
CPD_EXTERN void cpd_searchpath_add(const char* path);


//! @}


#endif // cpd_environment_h
