/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_message_include_h
#define cpd_message_include_h

#include "cpd_instance.h"

//! @defgroup message message
//! @brief The message part of cpd.
//! @details This part manages the messages.

//! @addtogroup message
//! @{

//! @brief The message.
//! @details  The message is a simple structure that describes a message.
typedef struct cpd_message
{
    cpd_tie*    tie;
    cpd_symbol* selector;
    cpd_list    list;
}cpd_message;

//! @brief The message function prototype.
//! @details The function is used to receive all messages.
typedef void (*cpd_message_hook)(cpd_instance* instance, cpd_message message);

//! @brief Sends a message.
//! @param instance The instance.
//! @param message The message.
CPD_EXTERN void cpd_instance_message_send(cpd_instance* instance, cpd_message message);

//! @brief Binds an instance to a tie.
//! @param instance The instance.
//! @param tie The tie to bind.
//! @param messagehook The set of message function.
CPD_EXTERN void cpd_instance_bind(cpd_instance* instance, cpd_tie* tie, cpd_message_hook messagehook);

//! @brief Unbinds an instance to a tie.
//! @param instance The instance.
//! @param tie The tie to unbind from.
CPD_EXTERN void cpd_instance_unbind(cpd_instance* instance, cpd_tie* tie);

//! @}


#endif // cpd_message_h
