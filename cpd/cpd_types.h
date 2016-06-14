/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_tie_h
#define cpd_tie_h

#include "cpd_def.h"
#include <stddef.h>

//! @defgroup types types
//! @brief The basic types of cpd.
//! @details The part of sources manages the basic types of the interface like the tie, the
//! symbol, the gpointer and a variant list of these three types.

//! @addtogroup types
//! @{


//! @defgroup gpointer gpointer
//! @brief The gpointer type (not implemented).

//! @addtogroup gpointer
//! @{

//! @brief The opaque type used for graphical pointer (not implemented).
typedef struct _gpointer cpd_gpointer;

//! @}




//! @defgroup tie tie
//! @brief The tie type.

//! @addtogroup tie
//! @{

//! @brief The opaque type used to bind and unbind cpd_instance with string characters.
typedef struct _symbol      cpd_tie;

//! @brief Creates an opaque tie that can be used to communicate within cpd.
//! @param name The name that will be used to bind.
//! @return The pointer to the tie.
CPD_EXTERN cpd_tie* cpd_tie_create(const char* name);

//! @brief Gets the name of a tie.
//! @param tie The tie.
//! @return The name of the tie.
CPD_EXTERN char const* cpd_tie_get_name(cpd_tie const* tie);

//! @}



//! @defgroup symbol symbol
//! @brief The symbol type.

//! @addtogroup symbol
//! @{

//! @brief The opaque type used for fast comparaison of string characters.
typedef struct _symbol      cpd_symbol;

//! @brief Creates an opaque tie that can be used fast comparaison of string characters.
//! @param name The name of the symbol.
//! @return The pointer to the symbol.
CPD_EXTERN cpd_symbol* cpd_symbol_create(const char* name);

//! @brief Gets the name of a symbol.
//! @param symbol The symbol.
//! @return The name of the symbol.
CPD_EXTERN char const* cpd_symbol_get_name(cpd_symbol const* symbol);

//! @}


//! @defgroup list list
//! @brief The list type.

//! @addtogroup list
//! @{

//! @brief The opaque list used for messages.
typedef struct cpd_list
{
    size_t  size;
    void*   vector;
} cpd_list;

//! @brief The type of messages in a list.
typedef enum
{
    CPD_NULL,     //!< @brief The element is undefined or null.
    CPD_FLOAT,    //!< @brief The element is a floating point number.
    CPD_SYMBOL,   //!< @brief The element is a symbol.
    CPD_POINTER   //!< @brief The element is a graphical pointer.
} cpd_listtype;

//! @brief Creates an opaque list that can be for messages within cpd.
//! @param list The pointer to the list.
//! @param size The size of the list.
CPD_EXTERN void cpd_list_init(cpd_list *list, size_t size);

//! @brief Frees a list.
//! @param list The pointer to the list.
CPD_EXTERN void cpd_list_clear(cpd_list *list);

//! @brief Gets the type of a data in the list.
//! @param list The pointer to the list.
//! @param index The index of the data.
//! @return The type of the data.
CPD_EXTERN cpd_listtype cpd_list_get_type(cpd_list const* list, size_t index);

//! @brief Gets the float value of a data of the list.
//! @param list The pointer to the list.
//! @param index The index of the data.
//! @return The float value of the data.
CPD_EXTERN float cpd_list_get_float(cpd_list const* list, size_t index);

//! @brief Gets the symbol of a data of the list.
//! @param list The pointer to the list.
//! @param index The index of the data.
//! @return The symbol of the data.
CPD_EXTERN cpd_symbol* cpd_list_get_symbol(cpd_list const* list, size_t index);

//! @brief Gets the gpointer of a data of the list.
//! @param list The pointer to the list.
//! @param index The index of the data.
//! @return The gpointer of the data.
CPD_EXTERN cpd_gpointer* cpd_list_get_gpointer(cpd_list const* list, size_t index);

//! @brief Sets the float value of a data of the list.
//! @param list The pointer to the list.
//! @param index The index of the data.
//! @param value The float value.
CPD_EXTERN void cpd_list_set_float(cpd_list *list, size_t index, float value);

//! @brief Sets the symbol of a data of the list.
//! @param list The pointer to the list.
//! @param index The index of the data.
//! @param symbol The symbol.
CPD_EXTERN void cpd_list_set_symbol(cpd_list *list, size_t index, cpd_symbol* symbol);

//! @brief Sets the gpointer of a data of the list.
//! @param list The pointer to the list.
//! @param index The index of the data.
//! @param pointer The gpointer.
CPD_EXTERN void cpd_list_set_gpointer(cpd_list *list, size_t index, cpd_gpointer* pointer);

//! @}
//! @}


#endif // cpd_tie_h
