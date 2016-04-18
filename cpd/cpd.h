/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_h
#define cpd_h

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#ifdef _WIN32
#ifdef PD_INTERNAL
#define CPD_EXTERN __declspec(dllexport) extern
#else
#define CPD_EXTERN __declspec(dllimport) extern
#endif
#else
#define CPD_EXTERN extern
#endif

#if defined(_MSC_VER) && !defined(_LANGUAGE_C_PLUS_PLUS) && !defined(__cplusplus)
#define CPD_EXTERN_STRUCT extern struct
#else
#define CPD_EXTERN_STRUCT struct
#endif

CPD_EXTERN_STRUCT _symbol;
CPD_EXTERN_STRUCT _symbol;
CPD_EXTERN_STRUCT _binbuf;
CPD_EXTERN_STRUCT _internal;
CPD_EXTERN_STRUCT _gpointer;
CPD_EXTERN_STRUCT _glist;
CPD_EXTERN_STRUCT _text;
CPD_EXTERN_STRUCT _iemgui;
CPD_EXTERN_STRUCT _atom;

//! @defgroup cpd The c interface od zpd.
//! @brief The low level c methods of the Pure Data wrapper

//! @addtogroup cpd
//! @{
//!

//! @brief The type used for samples during DSP.
typedef float c_sample;
//! @brief The type used to .
typedef float c_float;
//! @brief The tie type.
typedef struct _symbol      c_tie;
//! @brief The type used for fast comparaison of string characters.
typedef struct _symbol      c_symbol;

typedef struct _list        c_list;
typedef struct _internal    c_internal;
typedef struct _gpointer    c_gpointer;
typedef struct _array       c_array;
typedef struct _glist       c_patch;
typedef struct _text        c_object;
typedef struct _iemgui      c_gui;

typedef struct _instance
{
    c_internal* c_internal_ptr;
}c_instance;

//! @brief The type of messages in a list.
typedef enum
{
    Z_NULL,     //!< @brief Undefined or null
    Z_FLOAT,    //!< @brief Floating point number
    Z_SYMBOL,   //!< @brief Symbol
    Z_POINTER   //!< @brief Graphical pointer
} c_atomtype;

typedef struct _iatom
{
    c_atomtype  a_type;
    union _word
    {
        c_float     w_float;
        c_symbol*   w_symbol;
        c_gpointer* w_gpointer;
        c_array*    w_array;
        c_list*     w_list;
        int         w_index;
    }a_w;
}c_atom;



typedef void (*c_hook_bang)(struct _instance* instance, c_tie* tie);
typedef void (*c_hook_float)(struct _instance* instance, c_tie* tie, c_float f);
typedef void (*c_hook_symbol)(struct _instance* instance, c_tie* tie, c_symbol* s);
typedef void (*c_hook_gpointer)(struct _instance* instance, c_tie* tie, c_gpointer *gp);
typedef void (*c_hook_list)(struct _instance* instance, c_tie* tie, c_list *list);
typedef void (*c_hook_anything)(struct _instance* instance, c_tie* tie, c_symbol *s, c_list *list);


//! @brief The method prototype to receive messages from the console.
typedef void (*c_hook_print)(struct _instance* instance, const char *s);

//! @brief The console methods.
typedef struct _hook_console
{
    c_hook_print  m_post;   //!< @brief The post method
    c_hook_print  m_log;    //!< @brief The log method
    c_hook_print  m_error;  //!< @brief The error method
    c_hook_print  m_fatal;  //!< @brief The fatal method
}c_hook_console;

//! @brief The midi note on method prototype.
typedef void (*c_hook_noteon)(struct _instance* instance, int port, int channel, int pitch, int velocity);
//! @brief The midi control change method prototype.
typedef void (*c_hook_controlchange)(struct _instance* instance, int port, int channel, int control, int value);
//! @brief The midi progam change method prototype.
typedef void (*c_hook_programchange)(struct _instance* instance, int port, int channel, int value);
//! @brief The midi pitch bend method prototype.
typedef void (*c_hook_pitchbend)(struct _instance* instance, int port, int channel, int value);
//! @brief The midi after touch method prototype.
typedef void (*c_hook_aftertouch)(struct _instance* instance, int port, int channel, int value);
//! @brief The midi poly after touch method prototype.
typedef void (*c_hook_polyaftertouch)(struct _instance* instance, int port, int channel, int pitch, int value);
//! @brief The midi byte method prototype.
typedef void (*c_hook_byte)(struct _instance* instance, int port, int value);

//! @brief The midi methods.
typedef struct _hook_midi
{
    c_hook_noteon           m_noteon;           //!< brief The note on method
    c_hook_controlchange    m_controlchange;    //!< brief The control change method
    c_hook_programchange    m_programchange;    //!< brief The program change method
    c_hook_pitchbend        m_pitchbend;        //!< brief The pitch bend method
    c_hook_aftertouch       m_aftertouch;       //!< brief The after touch method
    c_hook_polyaftertouch   m_polyaftertouch;   //!< brief The poly after touch method
    c_hook_byte             m_byte;             //!< brief The byte method
}c_hook_midi;

//! \brief The messages methods.
typedef struct _hook_message
{
    c_hook_bang     m_bang;
    c_hook_float    m_float;
    c_hook_symbol   m_symbol;
    c_hook_gpointer m_gpointer;
    c_hook_list     m_list;
    c_hook_anything m_anything;
}c_hook_message;


//! @addtogroup enviroment
//! @{
//!

//! @brief Initializes the Pure Data environment.
//! @details The method should be called only one time at the initialization, before
//! everything else.
CPD_EXTERN void cpd_init();

//! @brief Clears the Pure Data environment.
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
CPD_EXTERN void cpd_searchpath_add(const char* path);




//! @brief Sends a normal post to the current instance.
CPD_EXTERN void cpd_console_post(char const* message, ...);

//! @brief Sends a log post to the current instance.
CPD_EXTERN void cpd_console_log(char const* message, ...);

//! @brief Sends an error to the current instance.
CPD_EXTERN void cpd_console_error(char const* message, ...);

//! @brief Sends a fatal error to the current instance.
CPD_EXTERN void cpd_console_fatal(char const* message, ...);


//! @}



//! @brief Creates a new instance.
//! @param The size of memory to allocate in bytes.
CPD_EXTERN c_instance* cpd_instance_new(size_t size);

//! @brief Deletes an instance.
CPD_EXTERN void cpd_instance_free(c_instance* instance);

//! @brief Sets the current instance.
CPD_EXTERN void cpd_instance_set(c_instance* instance);

//! @brief Sets the print methods of an instance.
CPD_EXTERN void cpd_instance_set_hook_console(c_instance* instance, c_hook_console* consolehook);

//! @brief Sets the midi methods of an instance.
CPD_EXTERN void cpd_instance_set_hook_midi(c_instance* instance, c_hook_midi* midihook);

//! @brief Binds an instance to a tie.
CPD_EXTERN void cpd_instance_bind(c_instance* instance, c_tie* tie, c_hook_message* messagehook);

//! @brief Unbinds an instance to a tie.
CPD_EXTERN void cpd_instance_unbind(c_instance* instance, c_tie* tie);

//! @brief Prepares the DSP for an instance.
CPD_EXTERN void cpd_instance_dsp_prepare(c_instance* instance,
                                           const int nins, const int nouts,
                                           const int samplerate, const int nsamples);

//! @brief Performs the DSP for an instance.
CPD_EXTERN void cpd_instance_dsp_perform(c_instance* instance, int nsamples,
                                           const int nins, const c_sample** inputs,
                                           const int nouts, c_sample** outputs);

//! @brief Releases the DSP for an instance.
CPD_EXTERN void cpd_instance_dsp_release(c_instance* instance);

//! @brief Gets the sample rate of an instance.
CPD_EXTERN int cpd_instance_get_samplerate(c_instance* instance);








//! @brief Creates a new patch.
CPD_EXTERN c_patch* cpd_patch_new(const char* name, const char* path);

//! @brief Frees a patch.
CPD_EXTERN void cpd_patch_free(c_patch* patch);

//! @brief Gets the name of a patch.
CPD_EXTERN const char* cpd_patch_get_name(c_patch const* patch);

//! @brief Gets the path of a patch.
CPD_EXTERN const char* cpd_patch_get_path(c_patch const* patch);

//! @brief Gets the dollar zero of a patch.
CPD_EXTERN int cpd_patch_get_dollarzero(c_patch const* patch);

//! @brief Gets the x margin of a patch.
CPD_EXTERN int cpd_patch_get_x(c_patch const* patch);

//! @brief Gets the x margin of a patch.
CPD_EXTERN int cpd_patch_get_y(c_patch const* patch);

//! @brief Gets the width of a patch.
CPD_EXTERN int cpd_patch_get_width(c_patch const* patch);

//! @brief Gets the height of a patch.
CPD_EXTERN int cpd_patch_get_height(c_patch const* patch);

//! @brief Gets the first object of a patch.
CPD_EXTERN c_object* cpd_patch_get_first_object(c_patch* patch);

//! @brief Gets the next object of a patch.
CPD_EXTERN c_object* cpd_patch_get_next_object(c_patch* patch, c_object* previous);




//! @brief Gets the name of an object.
CPD_EXTERN c_symbol* cpd_object_get_name(c_object const* object);

//! @brief Gets the name of an object.
CPD_EXTERN void cpd_object_get_text(c_object const* object, int* size, char** text);

//! @brief Gets the bounds of an object.
CPD_EXTERN void cpd_object_get_bounds(c_object const* object, c_patch const* patch,
                                        int* x, int* y, int* width, int* height);








//! @brief Gets the label of a gui.
CPD_EXTERN c_symbol* cpd_gui_get_label(c_gui const* gui);

//! @brief Gets the receive symbol of a gui.
CPD_EXTERN c_symbol* cpd_gui_get_receive_symbol(c_gui const* gui);

//! @brief Gets the send symbol of a gui.
CPD_EXTERN c_symbol* cpd_gui_get_send_symbol(c_gui const* gui);

//! @brief Gets the maximum value of a gui.
CPD_EXTERN float cpd_gui_get_maximum_value(c_gui const* gui);

//! @brief Gets the maximum value of a gui.
CPD_EXTERN float cpd_gui_get_minimum_value(c_gui const* gui);

//! @brief Gets the number of steps of a gui.
CPD_EXTERN int cpd_gui_get_number_of_steps(c_gui const* gui);

//! @brief Gets the number of steps of a gui.
CPD_EXTERN float cpd_gui_get_value(c_gui const* gui);

//! @brief Gets the bounds of a gui.
CPD_EXTERN void cpd_gui_get_label_position(c_gui const* gui, c_patch const* patch, int* x, int* y);






//! @brief Creates an opaque tie that can be understood by Pure Data.
CPD_EXTERN c_tie* cpd_tie_create(const char* name);

//! @brief Gets an opaque symbol that can be understood by Pure Data.
CPD_EXTERN char const* cpd_tie_get_name(c_tie const* tie);

//! @brief Creates an opaque symbol that can be understood by Pure Data.
CPD_EXTERN c_symbol* cpd_symbol_create(const char* symbol);

//! @brief Gets an opaque symbol that can be understood by Pure Data.
CPD_EXTERN char const* cpd_symbol_get_name(c_symbol const* symbol);



//! @brief Creates an opaque list that can be understood by Pure Data.
CPD_EXTERN c_list* cpd_list_create(size_t size);

//! @brief Clears a list.
CPD_EXTERN void cpd_list_free(c_list *list);

//! @brief Resizes a list.
CPD_EXTERN char cpd_list_resize(c_list *list, size_t size);

//! @brief Creates a copy of a list.
CPD_EXTERN c_list* cpd_list_create_copy(c_list const* list);

//! @brief Copy of a list into another.
CPD_EXTERN char cpd_list_copy(c_list* list1, c_list const* list2);

//! @brief Gets the size of a list.
CPD_EXTERN size_t cpd_list_get_size(c_list const* list);

//! @brief Gets the type of a data of the list.
CPD_EXTERN c_atomtype cpd_list_get_type(c_list const* list, size_t index);

//! @brief Gets the float value of a data of the list.
CPD_EXTERN c_float cpd_list_get_float(c_list const* list, size_t index);

//! @brief Gets the symbol of a data of the list.
CPD_EXTERN c_symbol* cpd_list_get_symbol(c_list const* list, size_t index);

//! @brief Gets the gpointer of a data of the list.
CPD_EXTERN c_gpointer* cpd_list_get_gpointer(c_list const* list, size_t index);

//! @brief Sets the float value of a data of the list.
CPD_EXTERN void cpd_list_set_float(c_list *list, size_t index, c_float value);

//! @brief Sets the symbol of a data of the list.
CPD_EXTERN void cpd_list_set_symbol(c_list *list, size_t index, c_symbol* symbol);

//! @brief Sets the gpointer of a data of the list.
CPD_EXTERN void cpd_list_set_gpointer(c_list *list, size_t index, c_gpointer* pointer);

CPD_EXTERN void* cpd_list_get_atom(c_list* list, size_t index);

CPD_EXTERN c_atomtype cpd_atom_get_type(c_atom const* atom);
CPD_EXTERN c_float cpd_atom_get_float(c_atom const* atom);
CPD_EXTERN c_symbol* cpd_atom_get_symbol(c_atom const* atom);
CPD_EXTERN void cpd_atom_set_float(c_atom* atom, c_float value);
CPD_EXTERN void cpd_atom_set_symbol(c_atom* atom, c_symbol* symbol);











//! @brief Sends a bang to Pure Data.
CPD_EXTERN void cpd_messagesend_bang(c_tie const* tie);

//! @brief Sends a float to Pure Data.
CPD_EXTERN void cpd_messagesend_float(c_tie const* tie, c_float value);

//! @brief Sends a gpointer to Pure Data.
CPD_EXTERN void cpd_messagesend_gpointer(c_tie const* tie, c_gpointer const* pointer);

//! @brief Sends a symbol to Pure Data.
CPD_EXTERN void cpd_messagesend_symbol(c_tie const* tie, c_symbol const* symbol);

//! @brief Sends a list to Pure Data.
CPD_EXTERN void cpd_messagesend_list(c_tie const* tie, c_list const* list);

//! @brief Sends a anything to Pure Data.
CPD_EXTERN void cpd_messagesend_anything(c_tie const* tie, c_symbol const* symbol, c_list const* list);







//! @brief Sends a midi note on event to Pure Data.
CPD_EXTERN void cpd_midisend_noteon(int channel, int pitch, int velocity);

//! @brief Sends a midi control change event to Pure Data.
CPD_EXTERN void cpd_midisend_controlchange(int channel, int controller, int value);

//! @brief Sends a midi program change event to Pure Data.
CPD_EXTERN void cpd_midisend_programchange(int channel, int value);

//! @brief Sends a midi pitch bend event to Pure Data.
CPD_EXTERN void cpd_midisend_pitchbend(int channel, int value);

//! @brief Sends a midi after touch event to Pure Data.
CPD_EXTERN void cpd_midisend_aftertouch(int channel, int value);

//! @brief Sends a midi poly after touch event to Pure Data.
CPD_EXTERN void cpd_midisend_polyaftertouch(int channel, int pitch, int value);

//! @brief Sends a midi byte event to Pure Data.
CPD_EXTERN void cpd_midisend_byte(int port, int byte);

//! @brief Sends a midi sys ex event to Pure Data.
CPD_EXTERN void cpd_midisend_sysex(int port, int byte);

//! @brief Sends a midi real time in event to Pure Data.
CPD_EXTERN void cpd_midisend_sysrealtimein(int port, int byte);

//! @}

#undef CPD_EXTERN
#undef CPD_EXTERN_STRUCT


#endif /* cpd_h */
