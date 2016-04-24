/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_patch_h
#define cpd_patch_h

#include "cpd_instance.h"


//! @defgroup patch patch
//! @brief The patch of cpd.

//! @addtogroup patch
//! @{

//! @brief Gets the name of a patch.
//! @param patch The patch.
//! @return The name of the patch.
CPD_EXTERN const char* cpd_patch_get_name(cpd_patch const* patch);

//! @brief Gets the path of a patch.
//! @param patch The patch.
//! @return The path of the patch.
CPD_EXTERN const char* cpd_patch_get_path(cpd_patch const* patch);

//! @brief Gets the dollar zero of a patch.
//! @param patch The patch.
//! @return The unique dollar zero id of the patch.
CPD_EXTERN int cpd_patch_get_dollarzero(cpd_patch const* patch);

//! @brief Gets the x margin of a patch.
//! @param patch The patch.
//! @return The x margin position of the patch.
CPD_EXTERN int cpd_patch_get_x(cpd_patch const* patch);

//! @brief Gets the x margin of a patch.
//! @param patch The patch.
//! @return The y margin position of the patch.
CPD_EXTERN int cpd_patch_get_y(cpd_patch const* patch);

//! @brief Gets the width of a patch.
//! @param patch The patch.
//! @return The width of the patch.
CPD_EXTERN int cpd_patch_get_width(cpd_patch const* patch);

//! @brief Gets the height of a patch.
//! @param patch The patch.
//! @return The height of the patch.
CPD_EXTERN int cpd_patch_get_height(cpd_patch const* patch);

//! @brief Gets the first object of a patch.
//! @param patch The patch.
//! @return The pointer to the first object of the patch.
CPD_EXTERN cpd_object* cpd_patch_get_first_object(cpd_patch const* patch);

//! @brief Gets the next object of a patch.
//! @param patch The patch.
//! @param previous The previous object retrieved from the patch.
//! @return The pointer to the next object of the patch if there is one, otherwise NULL.
CPD_EXTERN cpd_object* cpd_patch_get_next_object(cpd_patch const* patch, cpd_object const* previous);

//! @}



#endif // cpd_patch_h
