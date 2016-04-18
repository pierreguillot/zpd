/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef z_pd_h
#define z_pd_h

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#ifdef _WIN32
#ifdef PD_INTERNAL
#define Z_PD_EXTERN __declspec(dllexport) extern
#else
#define Z_PD_EXTERN __declspec(dllimport) extern
#endif
#else
#define Z_PD_EXTERN extern
#endif

#if defined(_MSC_VER) && !defined(_LANGUAGE_C_PLUS_PLUS) && !defined(__cplusplus)
#define Z_PD_EXTERN_STRUCT extern struct
#else
#define Z_PD_EXTERN_STRUCT struct
#endif

Z_PD_EXTERN_STRUCT _symbol;
Z_PD_EXTERN_STRUCT _symbol;
Z_PD_EXTERN_STRUCT _binbuf;
Z_PD_EXTERN_STRUCT _internal;
Z_PD_EXTERN_STRUCT _gpointer;
Z_PD_EXTERN_STRUCT _glist;
Z_PD_EXTERN_STRUCT _text;
Z_PD_EXTERN_STRUCT _iemgui;
Z_PD_EXTERN_STRUCT _atom;

//! @defgroup zpd The zpd part.
//! @brief The low level c methods of the Pure Data wrapper

//! @defgroup enviroment The global enviroment.
//! @brief The methods that manage the global Pure Data enviroment

//! @addtogroup zpd
//! @{
//!

//! @brief The type used for samples during DSP.
typedef float z_sample;
//! @brief The type used to .
typedef float z_float;
//! @brief The tie type.
typedef struct _symbol      z_tie;
//! @brief The type used for fast comparaison of string characters.
typedef struct _symbol      z_symbol;
//! @brief The atom type.
typedef struct _atom        z_atom;

typedef struct _list        z_list;
typedef struct _internal    z_internal;
typedef struct _gpointer    z_gpointer;
typedef struct _glist       z_patch;
typedef struct _text        z_object;
typedef struct _iemgui      z_gui;

typedef struct _instance
{
    z_internal* z_internal_ptr;
}z_instance;

//! @brief The type of messages in a list.
typedef enum
{
    Z_NULL,     //!< @brief Undefined or null
    Z_FLOAT,    //!< @brief Floating point number
    Z_SYMBOL,   //!< @brief Symbol
    Z_POINTER   //!< @brief Graphical pointer
} z_atomtype;





typedef void (*z_hook_bang)(struct _instance* instance, z_tie* tie);
typedef void (*z_hook_float)(struct _instance* instance, z_tie* tie, z_float f);
typedef void (*z_hook_symbol)(struct _instance* instance, z_tie* tie, z_symbol* s);
typedef void (*z_hook_gpointer)(struct _instance* instance, z_tie* tie, z_gpointer *gp);
typedef void (*z_hook_list)(struct _instance* instance, z_tie* tie, z_list *list);
typedef void (*z_hook_anything)(struct _instance* instance, z_tie* tie, z_symbol *s, z_list *list);


//! @brief The method prototype to receive messages from the console.
typedef void (*z_hook_print)(struct _instance* instance, const char *s);

//! @brief The console methods.
typedef struct _hook_console
{
    z_hook_print  m_post;   //!< @brief The post method
    z_hook_print  m_log;    //!< @brief The log method
    z_hook_print  m_error;  //!< @brief The error method
    z_hook_print  m_fatal;  //!< @brief The fatal method
}z_hook_console;

//! @brief The midi note on method prototype.
typedef void (*z_hook_noteon)(struct _instance* instance, int port, int channel, int pitch, int velocity);
//! @brief The midi control change method prototype.
typedef void (*z_hook_controlchange)(struct _instance* instance, int port, int channel, int control, int value);
//! @brief The midi progam change method prototype.
typedef void (*z_hook_programchange)(struct _instance* instance, int port, int channel, int value);
//! @brief The midi pitch bend method prototype.
typedef void (*z_hook_pitchbend)(struct _instance* instance, int port, int channel, int value);
//! @brief The midi after touch method prototype.
typedef void (*z_hook_aftertouch)(struct _instance* instance, int port, int channel, int value);
//! @brief The midi poly after touch method prototype.
typedef void (*z_hook_polyaftertouch)(struct _instance* instance, int port, int channel, int pitch, int value);
//! @brief The midi byte method prototype.
typedef void (*z_hook_byte)(struct _instance* instance, int port, int value);

//! @brief The midi methods.
typedef struct _hook_midi
{
    z_hook_noteon           m_noteon;           //!< brief The note on method
    z_hook_controlchange    m_controlchange;    //!< brief The control change method
    z_hook_programchange    m_programchange;    //!< brief The program change method
    z_hook_pitchbend        m_pitchbend;        //!< brief The pitch bend method
    z_hook_aftertouch       m_aftertouch;       //!< brief The after touch method
    z_hook_polyaftertouch   m_polyaftertouch;   //!< brief The poly after touch method
    z_hook_byte             m_byte;             //!< brief The byte method
}z_hook_midi;

//! \brief The messages methods.
typedef struct _hook_message
{
    z_hook_bang     m_bang;
    z_hook_float    m_float;
    z_hook_symbol   m_symbol;
    z_hook_gpointer m_gpointer;
    z_hook_list     m_list;
    z_hook_anything m_anything;
}z_hook_message;


//! @addtogroup enviroment
//! @{
//!

//! @brief Initializes the Pure Data environment.
//! @details The method should be called only one time at the initialization, before
//! everything else.
Z_PD_EXTERN void z_pd_init();

//! @brief Clears the Pure Data environment.
//! @details The method should be called only one time when the Pure Data environment will
//! never be called again.
Z_PD_EXTERN void z_pd_clear();




//! @brief Gets the major version of Pure Data.
Z_PD_EXTERN unsigned int z_pd_version_getmajor();

//! @brief Gets the minor version of Pure Data.
Z_PD_EXTERN unsigned int z_pd_version_getminor();

//! @brief Gets the bug version of Pure Data.
Z_PD_EXTERN unsigned int z_pd_version_getbug();




//! @brief Clears the search path of Pure Data.
//! @details Clears the search path for all the environment. The path will be used when you
//! try to open a patch, to load an abstraction or an external.
Z_PD_EXTERN void z_pd_searchpath_clear();

//! @brief Adds a path to the search path of Pure Data.
//! @details Adds a path to the search path for all the environment. The path will be used
//! when you try to open a patch, to load an abstraction or an external.
Z_PD_EXTERN void z_pd_searchpath_add(const char* path);




//! @brief Sends a normal post to the current instance.
Z_PD_EXTERN void z_pd_console_post(char const* message, ...);

//! @brief Sends a log post to the current instance.
Z_PD_EXTERN void z_pd_console_log(char const* message, ...);

//! @brief Sends an error to the current instance.
Z_PD_EXTERN void z_pd_console_error(char const* message, ...);

//! @brief Sends a fatal error to the current instance.
Z_PD_EXTERN void z_pd_console_fatal(char const* message, ...);


//! @}



//! @brief Creates a new instance.
//! @param The size of memory to allocate in bytes.
Z_PD_EXTERN z_instance* z_pd_instance_new(size_t size);

//! @brief Deletes an instance.
Z_PD_EXTERN void z_pd_instance_free(z_instance* instance);

//! @brief Sets the current instance.
Z_PD_EXTERN void z_pd_instance_set(z_instance* instance);

//! @brief Sets the print methods of an instance.
Z_PD_EXTERN void z_pd_instance_set_hook_console(z_instance* instance, z_hook_console* consolehook);

//! @brief Sets the midi methods of an instance.
Z_PD_EXTERN void z_pd_instance_set_hook_midi(z_instance* instance, z_hook_midi* midihook);

//! @brief Binds an instance to a tie.
Z_PD_EXTERN void z_pd_instance_bind(z_instance* instance, z_tie* tie, z_hook_message* messagehook);

//! @brief Unbinds an instance to a tie.
Z_PD_EXTERN void z_pd_instance_unbind(z_instance* instance, z_tie* tie);

//! @brief Prepares the DSP for an instance.
Z_PD_EXTERN void z_pd_instance_dsp_prepare(z_instance* instance,
                                           const int nins, const int nouts,
                                           const int samplerate, const int nsamples);

//! @brief Performs the DSP for an instance.
Z_PD_EXTERN void z_pd_instance_dsp_perform(z_instance* instance, int nsamples,
                                           const int nins, const z_sample** inputs,
                                           const int nouts, z_sample** outputs);

//! @brief Releases the DSP for an instance.
Z_PD_EXTERN void z_pd_instance_dsp_release(z_instance* instance);

//! @brief Gets the sample rate of an instance.
Z_PD_EXTERN int z_pd_instance_get_samplerate(z_instance* instance);








//! @brief Creates a new patch.
Z_PD_EXTERN z_patch* z_pd_patch_new(const char* name, const char* path);

//! @brief Frees a patch.
Z_PD_EXTERN void z_pd_patch_free(z_patch* patch);

//! @brief Gets the name of a patch.
Z_PD_EXTERN const char* z_pd_patch_get_name(z_patch const* patch);

//! @brief Gets the path of a patch.
Z_PD_EXTERN const char* z_pd_patch_get_path(z_patch const* patch);

//! @brief Gets the dollar zero of a patch.
Z_PD_EXTERN int z_pd_patch_get_dollarzero(z_patch const* patch);

//! @brief Gets the x margin of a patch.
Z_PD_EXTERN int z_pd_patch_get_x(z_patch const* patch);

//! @brief Gets the x margin of a patch.
Z_PD_EXTERN int z_pd_patch_get_y(z_patch const* patch);

//! @brief Gets the width of a patch.
Z_PD_EXTERN int z_pd_patch_get_width(z_patch const* patch);

//! @brief Gets the height of a patch.
Z_PD_EXTERN int z_pd_patch_get_height(z_patch const* patch);

//! @brief Gets the first object of a patch.
Z_PD_EXTERN z_object* z_pd_patch_get_first_object(z_patch* patch);

//! @brief Gets the next object of a patch.
Z_PD_EXTERN z_object* z_pd_patch_get_next_object(z_patch* patch, z_object* previous);




//! @brief Gets the name of an object.
Z_PD_EXTERN z_symbol* z_pd_object_get_name(z_object const* object);

//! @brief Gets the name of an object.
Z_PD_EXTERN void z_pd_object_get_text(z_object const* object, int* size, char** text);

//! @brief Gets the bounds of an object.
Z_PD_EXTERN void z_pd_object_get_bounds(z_object const* object, z_patch const* patch,
                                        int* x, int* y, int* width, int* height);








//! @brief Gets the label of a gui.
Z_PD_EXTERN z_symbol* z_pd_gui_get_label(z_gui const* gui);

//! @brief Gets the receive symbol of a gui.
Z_PD_EXTERN z_symbol* z_pd_gui_get_receive_symbol(z_gui const* gui);

//! @brief Gets the send symbol of a gui.
Z_PD_EXTERN z_symbol* z_pd_gui_get_send_symbol(z_gui const* gui);

//! @brief Gets the maximum value of a gui.
Z_PD_EXTERN float z_pd_gui_get_maximum_value(z_gui const* gui);

//! @brief Gets the maximum value of a gui.
Z_PD_EXTERN float z_pd_gui_get_minimum_value(z_gui const* gui);

//! @brief Gets the number of steps of a gui.
Z_PD_EXTERN int z_pd_gui_get_number_of_steps(z_gui const* gui);

//! @brief Gets the number of steps of a gui.
Z_PD_EXTERN float z_pd_gui_get_value(z_gui const* gui);

//! @brief Gets the bounds of a gui.
Z_PD_EXTERN void z_pd_gui_get_label_position(z_gui const* gui, z_patch const* patch, int* x, int* y);






//! @brief Creates an opaque tie that can be understood by Pure Data.
Z_PD_EXTERN z_tie* z_pd_tie_create(const char* name);

//! @brief Gets an opaque symbol that can be understood by Pure Data.
Z_PD_EXTERN char const* z_pd_tie_get_name(z_tie const* tie);

//! @brief Creates an opaque symbol that can be understood by Pure Data.
Z_PD_EXTERN z_symbol* z_pd_symbol_create(const char* symbol);

//! @brief Gets an opaque symbol that can be understood by Pure Data.
Z_PD_EXTERN char const* z_pd_symbol_get_name(z_symbol const* symbol);



//! @brief Creates an opaque list that can be understood by Pure Data.
Z_PD_EXTERN z_list* z_pd_list_create(size_t size);

//! @brief Clears a list.
Z_PD_EXTERN void z_pd_list_free(z_list *list);

//! @brief Resizes a list.
Z_PD_EXTERN char z_pd_list_resize(z_list *list, size_t size);

//! @brief Creates a copy of a list.
Z_PD_EXTERN z_list* z_pd_list_create_copy(z_list const* list);

//! @brief Copy of a list into another.
Z_PD_EXTERN char z_pd_list_copy(z_list* list1, z_list const* list2);

//! @brief Gets the size of a list.
Z_PD_EXTERN size_t z_pd_list_get_size(z_list const* list);

//! @brief Gets the type of a data of the list.
Z_PD_EXTERN z_atomtype z_pd_list_get_type(z_list const* list, size_t index);

//! @brief Gets the float value of a data of the list.
Z_PD_EXTERN z_float z_pd_list_get_float(z_list const* list, size_t index);

//! @brief Gets the symbol of a data of the list.
Z_PD_EXTERN z_symbol* z_pd_list_get_symbol(z_list const* list, size_t index);

//! @brief Gets the gpointer of a data of the list.
Z_PD_EXTERN z_gpointer* z_pd_list_get_gpointer(z_list const* list, size_t index);

//! @brief Sets the float value of a data of the list.
Z_PD_EXTERN void z_pd_list_set_float(z_list *list, size_t index, z_float value);

//! @brief Sets the symbol of a data of the list.
Z_PD_EXTERN void z_pd_list_set_symbol(z_list *list, size_t index, z_symbol* symbol);

//! @brief Sets the gpointer of a data of the list.
Z_PD_EXTERN void z_pd_list_set_gpointer(z_list *list, size_t index, z_gpointer* pointer);

Z_PD_EXTERN void* z_pd_list_get_atom(z_list* list, size_t index);

Z_PD_EXTERN z_atomtype cpd_atom_get_type(z_atom const* atom);
Z_PD_EXTERN z_float cpd_atom_get_float(z_atom const* atom);
Z_PD_EXTERN z_symbol* cpd_atom_get_symbol(z_atom const* atom);
Z_PD_EXTERN void cpd_atom_set_float(z_atom* atom, z_float value);
Z_PD_EXTERN void cpd_atom_set_symbol(z_atom* atom, z_symbol* symbol);











//! @brief Sends a bang to Pure Data.
Z_PD_EXTERN void z_pd_messagesend_bang(z_tie const* tie);

//! @brief Sends a float to Pure Data.
Z_PD_EXTERN void z_pd_messagesend_float(z_tie const* tie, z_float value);

//! @brief Sends a gpointer to Pure Data.
Z_PD_EXTERN void z_pd_messagesend_gpointer(z_tie const* tie, z_gpointer const* pointer);

//! @brief Sends a symbol to Pure Data.
Z_PD_EXTERN void z_pd_messagesend_symbol(z_tie const* tie, z_symbol const* symbol);

//! @brief Sends a list to Pure Data.
Z_PD_EXTERN void z_pd_messagesend_list(z_tie const* tie, z_list const* list);

//! @brief Sends a anything to Pure Data.
Z_PD_EXTERN void z_pd_messagesend_anything(z_tie const* tie, z_symbol const* symbol, z_list const* list);







//! @brief Sends a midi note on event to Pure Data.
Z_PD_EXTERN void z_pd_midisend_noteon(int channel, int pitch, int velocity);

//! @brief Sends a midi control change event to Pure Data.
Z_PD_EXTERN void z_pd_midisend_controlchange(int channel, int controller, int value);

//! @brief Sends a midi program change event to Pure Data.
Z_PD_EXTERN void z_pd_midisend_programchange(int channel, int value);

//! @brief Sends a midi pitch bend event to Pure Data.
Z_PD_EXTERN void z_pd_midisend_pitchbend(int channel, int value);

//! @brief Sends a midi after touch event to Pure Data.
Z_PD_EXTERN void z_pd_midisend_aftertouch(int channel, int value);

//! @brief Sends a midi poly after touch event to Pure Data.
Z_PD_EXTERN void z_pd_midisend_polyaftertouch(int channel, int pitch, int value);

//! @brief Sends a midi byte event to Pure Data.
Z_PD_EXTERN void z_pd_midisend_byte(int port, int byte);

//! @brief Sends a midi sys ex event to Pure Data.
Z_PD_EXTERN void z_pd_midisend_sysex(int port, int byte);

//! @brief Sends a midi real time in event to Pure Data.
Z_PD_EXTERN void z_pd_midisend_sysrealtimein(int port, int byte);

//! @}

#undef Z_PD_EXTERN
#undef Z_PD_EXTERN_STRUCT


#endif /* z_pd_h */
