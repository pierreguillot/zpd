/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_instance_h
#define cpd_instance_h

#include "cpd_environment.h"

//! @defgroup instance instance
//! @brief The instance of cpd.
//! @todo manage lock free & lock

//! @addtogroup instance
//! @{

//! @brief The opaque type used for internal instance management.
typedef struct _internal cpd_internal;

//! @brief The instance is the main interface to communicate within the cpd environment
//! @details The instance manages the posts to the console, the midi events, the messages
//! and the digital signal processing. It is also the interface to load and delete patches.
typedef struct _instance
{
    cpd_internal* cpd_internal_ptr;
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


//! @brief Loads a new patch.
//! @param instance The instance.
//! @param name The name of the patch.
//! @param path The path of the patch of NULL if the patch is in the searchpath.
//! @return The pointer to the patch or NULL if the patch has not been allocated.
CPD_EXTERN cpd_patch* cpd_instance_patch_load(cpd_instance* instance, const char* name, const char* path);

//! @brief Closes a patch.
//! @param instance The instance.
//! @param patch The patch.
CPD_EXTERN void cpd_instance_patch_close(cpd_instance* instance, cpd_patch* patch);



//! @defgroup dsp dsp
//! @brief The dsp part.

//! @addtogroup dsp
//! @{

//! @brief Prepares the digital signal processing for an instance.
//! @param instance The instance.
//! @param nins The number of inputs.
//! @param nouts The number of outputs.
//! @param samplerate The sample rate.
//! @param nsamples The number of samples per ticks.
CPD_EXTERN void cpd_instance_dsp_prepare(cpd_instance* instance, const int nins, const int nouts, const int samplerate, const int nsamples);

//! @brief Performs the digital signal processing for an instance.
//! @param instance The instance.
//! @param nsamples The number of samples.
//! @param nins The number of inputs.
//! @param inputs The input samples matrix.
//! @param nouts The number of outputs.
//! @param outputs The output samples matrix.
CPD_EXTERN void cpd_instance_dsp_perform(cpd_instance* instance, int nsamples, const int nins, const cpd_sample** inputs, const int nouts, cpd_sample** outputs);

//! @brief Releases the digital signal processing for an instance.
//! @param instance The instance.
CPD_EXTERN void cpd_instance_dsp_release(cpd_instance* instance);

//! @brief Gets the sample rate of an instance.
//! @param instance The instance.
//! @return The current sample rate of the instance.
CPD_EXTERN int cpd_instance_get_samplerate(cpd_instance* instance);

//! @}











//! @defgroup post post
//! @brief The console post part.

//! @addtogroup post
//! @{

//! @brief The method prototype to posts from the console.
typedef void (*cpd_hook_post)(struct _instance* instance, const char *s);

//! @brief The console methods.
typedef struct _hook_console
{
    cpd_hook_post  m_log;    //!< @brief The log post method
    cpd_hook_post  m_normal; //!< @brief The normal post method
    cpd_hook_post  m_error;  //!< @brief The error post method
    cpd_hook_post  m_fatal;  //!< @brief The fatal post method
}cpd_hook_console;

//! @brief Sets the print methods of an instance.
//! @param instance The instance.
//! @param consolehook The set of post methods.
CPD_EXTERN void cpd_instance_set_hook_console(cpd_instance* instance, cpd_hook_console* consolehook);

//! @brief Sends a normal post to the current instance.
//! @param instance The instance.
//! @param message The message in the printf format.
CPD_EXTERN void cpd_instance_post_normal(cpd_instance* instance, char const* message, ...);

//! @brief Sends a log post to the current instance.
//! @param instance The instance.
//! @param message The message in the printf format.
CPD_EXTERN void cpd_instance_post_log(cpd_instance* instance, char const* message, ...);

//! @brief Sends an error to the current instance.
//! @param instance The instance.
//! @param message The message in the printf format.
CPD_EXTERN void cpd_instance_post_error(cpd_instance* instance, char const* message, ...);

//! @brief Sends a fatal error to the current instance.
//! @param instance The instance.
//! @param message The message in the printf format.
CPD_EXTERN void cpd_instance_post_fatal(cpd_instance* instance, char const* message, ...);

//! @}





//! @defgroup message message
//! @brief The message part.

//! @addtogroup message
//! @{

//! @brief The bang method prototype.
typedef void (*cpd_hook_bang)(struct _instance* instance, cpd_tie* tie);
//! @brief The float method prototype.
typedef void (*cpd_hook_float)(struct _instance* instance, cpd_tie* tie, float f);
//! @brief The symbol method prototype.
typedef void (*cpd_hook_symbol)(struct _instance* instance, cpd_tie* tie, cpd_symbol* s);
//! @brief The gpointer method prototype.
typedef void (*cpd_hook_gpointer)(struct _instance* instance, cpd_tie* tie, cpd_gpointer const*gp);
//! @brief The list method prototype.
typedef void (*cpd_hook_list)(struct _instance* instance, cpd_tie* tie, cpd_list const* list);
//! @brief The anything method prototype.
typedef void (*cpd_hook_anything)(struct _instance* instance, cpd_tie* tie, cpd_symbol* s, cpd_list const* list);

//! @brief The messages methods.
typedef struct _hook_message
{
    cpd_hook_bang     m_bang;     //!< @brief The bang method
    cpd_hook_float    m_float;    //!< @brief The float method
    cpd_hook_symbol   m_symbol;   //!< @brief The symbol method
    cpd_hook_gpointer m_gpointer; //!< @brief The gpointer method
    cpd_hook_list     m_list;     //!< @brief The list method
    cpd_hook_anything m_anything; //!< @brief The anything method
}cpd_hook_message;

//! @brief Sends a bang through a tie.
//! @param instance The instance.
//! @param tie The tie.
CPD_EXTERN void cpd_instance_send_bang(cpd_instance* instance, cpd_tie const* tie);

//! @brief Sends a float through a tie.
//! @param instance The instance.
//! @param tie The tie.
//! @param value The float value.
CPD_EXTERN void cpd_instance_send_float(cpd_instance* instance, cpd_tie const* tie, float value);

//! @brief Sends a gpointer through a tie.
//! @param instance The instance.
//! @param tie The tie.
//! @param pointer The gpointer.
CPD_EXTERN void cpd_instance_send_gpointer(cpd_instance* instance, cpd_tie const* tie, cpd_gpointer const* pointer);

//! @brief Sends a symbol through a tie.
//! @param instance The instance.
//! @param tie The tie.
//! @param symbol The symbol.
CPD_EXTERN void cpd_instance_send_symbol(cpd_instance* instance, cpd_tie const* tie, cpd_symbol const* symbol);

//! @brief Sends a list through a tie.
//! @param instance The instance.
//! @param tie The tie.
//! @param list The list.
CPD_EXTERN void cpd_instance_send_list(cpd_instance* instance, cpd_tie const* tie, cpd_list const* list);

//! @brief Sends anything through a tie.
//! @param instance The instance.
//! @param tie The tie.
//! @param selector The selector.
//! @param list The list.
CPD_EXTERN void cpd_instance_send_anything(cpd_instance* instance, cpd_tie const* tie, cpd_symbol const* selector, cpd_list const* list);

//! @brief Binds an instance to a tie.
//! @param instance The instance.
//! @param tie The tie to bind to.
//! @param messagehook The set of message methods.
CPD_EXTERN void cpd_instance_bind(cpd_instance* instance, cpd_tie* tie, cpd_hook_message const* messagehook);

//! @brief Unbinds an instance to a tie.
//! @param instance The instance.
//! @param tie The tie to unbind from.
CPD_EXTERN void cpd_instance_unbind(cpd_instance* instance, cpd_tie* tie);

//! @}







//! @defgroup midi midi
//! @brief The midi part.

//! @addtogroup midi
//! @{

//! @brief The midi note on method prototype.
typedef void (*cpd_hook_noteon)(struct _instance* instance, int port, int channel, int pitch, int velocity);
//! @brief The midi control change method prototype.
typedef void (*cpd_hook_controlchange)(struct _instance* instance, int port, int channel, int control, int value);
//! @brief The midi progam change method prototype.
typedef void (*cpd_hook_programchange)(struct _instance* instance, int port, int channel, int value);
//! @brief The midi pitch bend method prototype.
typedef void (*cpd_hook_pitchbend)(struct _instance* instance, int port, int channel, int value);
//! @brief The midi after touch method prototype.
typedef void (*cpd_hook_aftertouch)(struct _instance* instance, int port, int channel, int value);
//! @brief The midi poly after touch method prototype.
typedef void (*cpd_hook_polyaftertouch)(struct _instance* instance, int port, int channel, int pitch, int value);
//! @brief The midi byte method prototype.
typedef void (*cpd_hook_byte)(struct _instance* instance, int port, int value);

//! @brief The midi methods.
typedef struct _hook_midi
{
    cpd_hook_noteon           m_noteon;           //!< brief The note on method
    cpd_hook_controlchange    m_controlchange;    //!< brief The control change method
    cpd_hook_programchange    m_programchange;    //!< brief The program change method
    cpd_hook_pitchbend        m_pitchbend;        //!< brief The pitch bend method
    cpd_hook_aftertouch       m_aftertouch;       //!< brief The after touch method
    cpd_hook_polyaftertouch   m_polyaftertouch;   //!< brief The poly after touch method
    cpd_hook_byte             m_byte;             //!< brief The byte method
}cpd_hook_midi;

//! @brief Sets the midi methods of an instance.
//! @param instance The instance.
//! @param midihook The set of midi methods.
CPD_EXTERN void cpd_instance_set_hook_midi(cpd_instance* instance, cpd_hook_midi* midihook);

//! @brief Sends a midi note on event.
//! @param instance The instance.
//! @param channel The channel between 0 and 16.
//! @param pitch The channel between 0 and 127.
//! @param velocity The velocity between 0 and 127.
CPD_EXTERN void cpd_instance_midi_noteon(cpd_instance* instance, int channel, int pitch, int velocity);

//! @brief Sends a midi control change event.
//! @param instance The instance.
//! @param channel The channel between 0 and 16.
//! @param controller The controller between 0 and 127.
//! @param value The value between 0 and 127.
CPD_EXTERN void cpd_instance_midi_controlchange(cpd_instance* instance, int channel, int controller, int value);

//! @brief Sends a midi program change event.
//! @param instance The instance.
//! @param channel The channel between 0 and 16.
//! @param value The value between 0 and 127.
CPD_EXTERN void cpd_instance_midi_programchange(cpd_instance* instance, int channel, int value);

//! @brief Sends a midi pitch bend event.
//! @param instance The instance.
//! @param channel The channel between 0 and 16.
//! @param value The value between 0 and 127.
CPD_EXTERN void cpd_instance_midi_pitchbend(cpd_instance* instance, int channel, int value);

//! @brief Sends a midi after touch event.
//! @param instance The instance.
//! @param channel The channel between 0 and 16.
//! @param value The value between 0 and 127.
CPD_EXTERN void cpd_instance_midi_aftertouch(cpd_instance* instance, int channel, int value);

//! @brief Sends a midi poly after touch event.
//! @param instance The instance.
//! @param channel The channel between 0 and 16.
//! @param pitch The channel between 0 and 127.
//! @param value The value between 0 and 127.
CPD_EXTERN void cpd_instance_midi_polyaftertouch(cpd_instance* instance, int channel, int pitch, int value);

//! @brief Sends a midi byte event.
//! @param instance The instance.
//! @param port The port.
//! @param byte The byte value.
CPD_EXTERN void cpd_instance_midi_byte(cpd_instance* instance, int port, int byte);

//! @}

//! @}



#endif // cpd_instance_h
