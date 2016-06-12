/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_midi_h
#define cpd_midi_h

#include "cpd_instance.h"

//! @defgroup midi midi
//! @brief The midi part of cpd.
//! @details This part manages the midi events.

//! @addtogroup midi
//! @{

//! @brief The type of midi event.
typedef enum
{
    CPD_MIDI_NOTE    = 0,    //!< @brief The note event.
    CPD_MIDI_CTRL    = 1,    //!< @brief The control change event.
    CPD_MIDI_PGRM    = 2,    //!< @brief The program change event.
    CPD_MIDI_BEND    = 3,    //!< @brief The pitch bend event.
    CPD_MIDI_ATOUCH  = 4,    //!< @brief The after touch event.
    CPD_MIDI_PATOUCH = 5,    //!< @brief The poly after touch event.
    CPD_MIDI_BYTE    = 6     //!< @brief The byte.
} cpd_midi_type;

//! @brief The data of a midi event.
typedef int cpd_midi_data;

//! @brief The midi event.
//! @details  Depending on the type of the midi events the data have different significations.
//! For the note event, the data are channel, pitch, velocity.
//! For the control change event, the data are channel, controller number, value.
//! For the program change event, the data are channel, program number.
//! For the pitch bend event, the data are channel, pitch bend between -8192 and 8192.
//! For the after touch event, the data are channel, unused data, touch value.
//! For the poly after touch event, the data are channel, pitch, touch value.
//! For the byte event, the port, unused data, byte value.
typedef struct cpd_midi_event
{
    cpd_midi_type type;     //!< @brief The type of midi event.
    cpd_midi_data data1;    //!< @brief The first data that depends on the midi type.
    cpd_midi_data data2;    //!< @brief The second data that depends on the midi type.
    cpd_midi_data data3;    //!< @brief The third data that depends on the midi type.
}cpd_midi_event;

//! @brief The midi function prototype.
//! @details The function is used to receive all the midi events.
//! @param instance The instance.
//! @param event The midi event.
typedef void (*cpd_midi_hook)(cpd_instance* instance, cpd_midi_event event);

//! @brief Sets the midi funtion of an instance.
//! @param instance The instance.
//! @param midihook The midi function.
CPD_EXTERN void cpd_instance_midi_sethook(cpd_instance* instance, cpd_midi_hook midihook);

//! @brief Sends a midi event.
//! @param instance The instance.
//! @param event The midi event.
CPD_EXTERN void cpd_instance_midi_send(cpd_instance* instance, cpd_midi_event event);

//! @}

#endif // cpd_midi_h
