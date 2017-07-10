/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_instance_include_h
#define cpd_instance_include_h

#include "cpd_environment.h"

//! @defgroup instance instance
//! @brief The instance of cpd.

//! @addtogroup instance
//! @{

CPD_EXTERN_STRUCT _pdinstance;
CPD_EXTERN_STRUCT cpd_dsp_manager;
CPD_EXTERN_STRUCT cpd_message_manager;
CPD_EXTERN_STRUCT cpd_midi_manager;
CPD_EXTERN_STRUCT cpd_post_manager;

//! @brief The instance is the main interface to communicate within the cpd environment
//! @details The instance manages the posts to the console, the midi events, the messages
//! and the digital signal processing. It is also the interface to load and delete patches.
typedef struct cpd_instance
{
    struct _pdinstance*         c_internal;
    struct cpd_message_manager* c_message;
    struct cpd_midi_manager*    c_midi;
    struct cpd_post_manager*    c_post;
}cpd_instance;


//! @brief Creates a new instance.
//! @details If you want to implement your own instance, the first memeber of the structure
//! must be a cpd_instance, after this you can add everything you want. You should allocate
//! your instance with this function using sizeof(myinstance).
//! @code{.c}
//! struct myinstance
//! {
//!     cpd_instance instance;
//!     int* values;
//! };
//! ...
//! myinstance* inst = cpd_instance_new(sizeof(myinstance));
//! if(inst)
//!     inst->value = (float *)malloc(512 * sizeof(float));
//! ...
//! @endcode
//! @param size The size of memory to allocate in bytes.
//! @return A pointer to the initialized cpd_instance or NULL if the allocation failed.
CPD_EXTERN cpd_instance* cpd_instance_new(size_t size);

//! @brief Deletes an instance.
//! @details You must first delete the members of your instance if need and therefater call
//! this method to free the instance.
//! @code{.c}
//! struct myinstance
//! {
//!     cpd_instance instance;
//!     int* values;
//! };
//! ...
//! myinstance* inst;
//! ..
//! free(inst->value);
//! cpd_instance_free(inst);
//! ...
//! @endcode
//! @param instance The pointer to the instance.
CPD_EXTERN void cpd_instance_free(cpd_instance* instance);


//! @brief Clears the search path of a specific instance.
//! @details Clears the search path for a specific instance. The path will be used when you
//! try to open a patch, to load an abstraction or an external.
CPD_EXTERN void cpd_instance_searchpath_clear(cpd_instance* instance);

//! @brief Adds a path to the search path of a specific instance.
//! @details Adds a path to the search path a specific instance. The path will be used
//! when you try to open a patch, to load an abstraction or an external.
//! @param path The path to add.
CPD_EXTERN void cpd_instance_searchpath_add(cpd_instance* instance, const char* path);

//! @}



#endif // cpd_instance_include_h
