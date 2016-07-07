/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_object_include_h
#define cpd_object_include_h

#include "cpd_patch.h"


//! @defgroup object object
//! @brief The object of cpd.

//! @addtogroup object
//! @{

//! @brief Gets the name of an object.
//! @param object The object.
//! @return The name of the object.
CPD_EXTERN cpd_symbol* cpd_object_get_name(cpd_object const* object);

//! @brief Gets the name of an object.
//! @param object The object.
//! @param size The size of the c-string character that will be allocated.
//! @param text The c-string character that will be allocated.
CPD_EXTERN void cpd_object_get_text(cpd_object const* object, int* size, char** text);

//! @brief Gets the x position of an object within a patch.
//! @param object The object.
//! @param patch The patch.
//! @return The x position of the object.
CPD_EXTERN int cpd_object_get_x(cpd_object const* object, cpd_patch const* patch);

//! @brief Gets the y position of an object within a patch.
//! @param object The object.
//! @param patch The patch.
//! @return The y position of the object.
CPD_EXTERN int cpd_object_get_y(cpd_object const* object, cpd_patch const* patch);

//! @brief Gets the width of an object within a patch.
//! @param object The object.
//! @param patch The patch.
//! @return The width of the object.
CPD_EXTERN int cpd_object_get_width(cpd_object const* object, cpd_patch const* patch);

//! @brief Gets the height of an object within a patch.
//! @param object The object.
//! @param patch The patch.
//! @return The height of the object.
CPD_EXTERN int cpd_object_get_height(cpd_object const* object, cpd_patch const* patch);

//! @brief Gets the if an object is a gui.
//! @param object The object.
//! @return A non null value if the object is a gui, otherwise zero.
CPD_EXTERN char cpd_object_is_gui(cpd_object const* object);

//! @}

#endif // cpd_object_include_h
