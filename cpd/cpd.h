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

//! @defgroup cpd cpd
//! @brief The low level c interface of zpd.
//! @details The cpd interface is

//! @addtogroup cpd
//! @{

//! @brief The type used for samples during digital signal processing.
typedef float cpd_sample;
//! @brief The type used for floating point numbers.
typedef float cpd_float;
//! @brief The opaque type used to bind and unbind cpd_instance with string characters.
typedef struct _symbol      cpd_tie;
//! @brief The opaque type used for fast comparaison of string characters.
typedef struct _symbol      cpd_symbol;
//! @brief The opaque type used for internal messages.
typedef struct _list        cpd_list;
//! @brief The opaque type used for internal instance management.
typedef struct _internal    cpd_internal;
//! @brief The opaque type used for graphical pointer (not implemented).
typedef struct _gpointer    cpd_gpointer;
//! @brief The opaque type used for a patch.
typedef struct _glist       cpd_patch;
//! @brief The opaque type used for an object.
typedef struct _text        cpd_object;
//! @brief The opaque type used for a iem gui object.
typedef struct _iemgui      cpd_gui;
//! @brief The instance is the main interface to communicate within the cpd environment
//! @details The instance manages the posts to the console, the midi events, the messages
//! and the digital signal processing. It is also the interface to load and delete patches.
typedef struct _instance
{
    cpd_internal* cpd_internal_ptr;
}cpd_instance;


//! @brief The type of messages in a list.
typedef enum
{
    Z_NULL,     //!< @brief Undefined or null
    Z_FLOAT,    //!< @brief Floating point number
    Z_SYMBOL,   //!< @brief Symbol
    Z_POINTER   //!< @brief Graphical pointer
} c_atomtype;

typedef enum
{
    Z_GUI_BANG          = 0,
    Z_GUI_SLIDERH       = 1,
    Z_GUI_SLIDERV       = 2,
    Z_GUI_TOGGLE        = 3,
    Z_GUI_NUMBER        = 4,
    Z_GUI_RADIOH        = 5,
    Z_GUI_RADIOV        = 6,
    Z_GUI_VUMETER       = 7,
    Z_GUI_PANEL         = 8
} cpd_guitype;



typedef void (*c_hook_bang)(struct _instance* instance, cpd_tie* tie);
typedef void (*c_hook_float)(struct _instance* instance, cpd_tie* tie, cpd_float f);
typedef void (*c_hook_symbol)(struct _instance* instance, cpd_tie* tie, cpd_symbol* s);
typedef void (*c_hook_gpointer)(struct _instance* instance, cpd_tie* tie, cpd_gpointer *gp);
typedef void (*c_hook_list)(struct _instance* instance, cpd_tie* tie, cpd_list *list);
typedef void (*c_hook_anything)(struct _instance* instance, cpd_tie* tie, cpd_symbol *s, cpd_list *list);


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





//! @brief Creates a new instance.
//! @param The size of memory to allocate in bytes.
CPD_EXTERN cpd_instance* cpd_instance_new(size_t size);

//! @brief Deletes an instance.
CPD_EXTERN void cpd_instance_free(cpd_instance* instance);

//! @brief Sets the current instance.
CPD_EXTERN void cpd_instance_set(cpd_instance* instance);

//! @brief Sets the print methods of an instance.
CPD_EXTERN void cpd_instance_set_hook_console(cpd_instance* instance, c_hook_console* consolehook);

//! @brief Sets the midi methods of an instance.
CPD_EXTERN void cpd_instance_set_hook_midi(cpd_instance* instance, c_hook_midi* midihook);

//! @brief Binds an instance to a tie.
CPD_EXTERN void cpd_instance_bind(cpd_instance* instance, cpd_tie* tie, c_hook_message* messagehook);

//! @brief Unbinds an instance to a tie.
CPD_EXTERN void cpd_instance_unbind(cpd_instance* instance, cpd_tie* tie);

//! @brief Prepares the DSP for an instance.
CPD_EXTERN void cpd_instance_dsp_prepare(cpd_instance* instance,
                                           const int nins, const int nouts,
                                           const int samplerate, const int nsamples);

//! @brief Performs the DSP for an instance.
CPD_EXTERN void cpd_instance_dsp_perform(cpd_instance* instance, int nsamples,
                                           const int nins, const cpd_sample** inputs,
                                           const int nouts, cpd_sample** outputs);

//! @brief Releases the DSP for an instance.
CPD_EXTERN void cpd_instance_dsp_release(cpd_instance* instance);

//! @brief Gets the sample rate of an instance.
CPD_EXTERN int cpd_instance_get_samplerate(cpd_instance* instance);








//! @brief Creates a new patch.
CPD_EXTERN cpd_patch* cpd_patch_new(const char* name, const char* path);

//! @brief Frees a patch.
CPD_EXTERN void cpd_patch_free(cpd_patch* patch);

//! @brief Gets the name of a patch.
CPD_EXTERN const char* cpd_patch_get_name(cpd_patch const* patch);

//! @brief Gets the path of a patch.
CPD_EXTERN const char* cpd_patch_get_path(cpd_patch const* patch);

//! @brief Gets the dollar zero of a patch.
CPD_EXTERN int cpd_patch_get_dollarzero(cpd_patch const* patch);

//! @brief Gets the x margin of a patch.
CPD_EXTERN int cpd_patch_get_x(cpd_patch const* patch);

//! @brief Gets the x margin of a patch.
CPD_EXTERN int cpd_patch_get_y(cpd_patch const* patch);

//! @brief Gets the width of a patch.
CPD_EXTERN int cpd_patch_get_width(cpd_patch const* patch);

//! @brief Gets the height of a patch.
CPD_EXTERN int cpd_patch_get_height(cpd_patch const* patch);

//! @brief Gets the first object of a patch.
CPD_EXTERN cpd_object* cpd_patch_get_first_object(cpd_patch const* patch);

//! @brief Gets the next object of a patch.
CPD_EXTERN cpd_object* cpd_patch_get_next_object(cpd_patch const* patch, cpd_object const* previous);




//! @brief Gets the name of an object.
CPD_EXTERN cpd_symbol* cpd_object_get_name(cpd_object const* object);

//! @brief Gets the name of an object.
CPD_EXTERN void cpd_object_get_text(cpd_object const* object, int* size, char** text);

CPD_EXTERN void cpd_object_get_bounds(cpd_object const* object, cpd_patch const* patch, int* x, int* y, int* width, int* height);

//! @brief Gets the x position of an object.
CPD_EXTERN int cpd_object_get_x(cpd_object const* object, cpd_patch const* patch);

//! @brief Gets the x position of an object.
CPD_EXTERN int cpd_object_get_y(cpd_object const* object, cpd_patch const* patch);

//! @brief Gets the width of an object.
CPD_EXTERN int cpd_object_get_width(cpd_object const* object, cpd_patch const* patch);

//! @brief Gets the height of an object.
CPD_EXTERN int cpd_object_get_height(cpd_object const* object, cpd_patch const* patch);

//! @brief Gets the if an object is a gui.
CPD_EXTERN char cpd_object_is_gui(cpd_object const* object);





//! @brief Gets the label of a gui.
CPD_EXTERN cpd_symbol* cpd_gui_get_label(cpd_gui const* gui);

//! @brief Gets the receive symbol of a gui.
CPD_EXTERN cpd_tie* cpd_gui_get_receive_tie(cpd_gui const* gui);

//! @brief Gets the send symbol of a gui.
CPD_EXTERN cpd_tie* cpd_gui_get_send_tie(cpd_gui const* gui);

//! @brief Gets the send symbol of a gui.
CPD_EXTERN cpd_guitype cpd_gui_get_type(cpd_gui const* gui);

//! @brief Gets the maximum value of a gui.
CPD_EXTERN float cpd_gui_get_maximum_value(cpd_gui const* gui);

//! @brief Gets the maximum value of a gui.
CPD_EXTERN float cpd_gui_get_minimum_value(cpd_gui const* gui);

//! @brief Gets the number of steps of a gui.
CPD_EXTERN int cpd_gui_get_number_of_steps(cpd_gui const* gui);

//! @brief Gets the number of steps of a gui.
CPD_EXTERN float cpd_gui_get_value(cpd_gui const* gui);

//! @brief Gets the x position of the label of a gui.
CPD_EXTERN int cpd_gui_get_label_x(cpd_gui const* gui, cpd_patch const* patch);

//! @brief Gets the y position of the label of a gui.
CPD_EXTERN int cpd_gui_get_label_y(cpd_gui const* gui, cpd_patch const* patch);






//! @brief Creates an opaque tie that can be understood by Pure Data.
CPD_EXTERN cpd_tie* cpd_tie_create(const char* name);

//! @brief Gets an opaque symbol that can be understood by Pure Data.
CPD_EXTERN char const* cpd_tie_get_name(cpd_tie const* tie);

//! @brief Creates an opaque symbol that can be understood by Pure Data.
CPD_EXTERN cpd_symbol* cpd_symbol_create(const char* symbol);

//! @brief Gets an opaque symbol that can be understood by Pure Data.
CPD_EXTERN char const* cpd_symbol_get_name(cpd_symbol const* symbol);



//! @brief Creates an opaque list that can be understood by Pure Data.
CPD_EXTERN cpd_list* cpd_list_create(size_t size);

//! @brief Clears a list.
CPD_EXTERN void cpd_list_free(cpd_list *list);

//! @brief Gets the size of a list.
CPD_EXTERN size_t cpd_list_get_size(cpd_list const* list);

//! @brief Gets the type of a data of the list.
CPD_EXTERN c_atomtype cpd_list_get_type(cpd_list const* list, size_t index);

//! @brief Gets the float value of a data of the list.
CPD_EXTERN cpd_float cpd_list_get_float(cpd_list const* list, size_t index);

//! @brief Gets the symbol of a data of the list.
CPD_EXTERN cpd_symbol* cpd_list_get_symbol(cpd_list const* list, size_t index);

//! @brief Gets the gpointer of a data of the list.
CPD_EXTERN cpd_gpointer* cpd_list_get_gpointer(cpd_list const* list, size_t index);

//! @brief Sets the float value of a data of the list.
CPD_EXTERN void cpd_list_set_float(cpd_list *list, size_t index, cpd_float value);

//! @brief Sets the symbol of a data of the list.
CPD_EXTERN void cpd_list_set_symbol(cpd_list *list, size_t index, cpd_symbol* symbol);

//! @brief Sets the gpointer of a data of the list.
CPD_EXTERN void cpd_list_set_gpointer(cpd_list *list, size_t index, cpd_gpointer* pointer);

CPD_EXTERN void* cpd_list_get_atom(cpd_list* list, size_t index);











//! @brief Sends a bang to Pure Data.
CPD_EXTERN void cpd_messagesend_bang(cpd_tie const* tie);

//! @brief Sends a float to Pure Data.
CPD_EXTERN void cpd_messagesend_float(cpd_tie const* tie, cpd_float value);

//! @brief Sends a gpointer to Pure Data.
CPD_EXTERN void cpd_messagesend_gpointer(cpd_tie const* tie, cpd_gpointer const* pointer);

//! @brief Sends a symbol to Pure Data.
CPD_EXTERN void cpd_messagesend_symbol(cpd_tie const* tie, cpd_symbol const* symbol);

//! @brief Sends a list to Pure Data.
CPD_EXTERN void cpd_messagesend_list(cpd_tie const* tie, cpd_list const* list);

//! @brief Sends a anything to Pure Data.
CPD_EXTERN void cpd_messagesend_anything(cpd_tie const* tie, cpd_symbol const* symbol, cpd_list const* list);







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

//! @}

#undef CPD_EXTERN
#undef CPD_EXTERN_STRUCT


#endif /* cpd_h */
