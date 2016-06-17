/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_post_h
#define cpd_post_h

#include "cpd_instance.h"

//! @defgroup post post
//! @brief The console post part of cpd.
//! @details This part manages the console posts.

//! @addtogroup post
//! @{

typedef enum cpd_postlevel
{
    cpd_post_fatal   = 0,   //!< @brief The fatal error posts.
    cpd_post_error   = 1,   //!< @brief The error posts.
    cpd_post_normal  = 2,   //!< @brief The normal posts.
    cpd_post_log     = 3    //!< @brief The log posts.
}cpd_postlevel;

//! @brief The post.
//! @details  The post is a simple structure that defines
typedef struct cpd_post
{
    cpd_postlevel level;
    const char*   text;
}cpd_post;

//! @brief The post function prototype.
//! @details The function is used to receive all posts from the console.
typedef void (*cpd_hook_post)(cpd_instance* instance, cpd_post);


//! @brief Sets the post function of an instance.
//! @param instance The instance.
//! @param posthook The post function.
CPD_EXTERN void cpd_instance_post_sethook(cpd_instance* instance, cpd_hook_post posthook);

//! @brief Sends a normal post to the current instance.
//! @param instance The instance.
//! @param post     The post message.
CPD_EXTERN void cpd_instance_post_send(cpd_instance* instance, cpd_post post);

//! @}


#endif // cpd_post_h
