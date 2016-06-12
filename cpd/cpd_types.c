/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.


#include "cpd_types.h"
#include "../pd/src/m_pd.h"
#include <stdlib.h>


cpd_tie* cpd_tie_create(const char* name)
{
    return (cpd_tie *)gensym(name);
}

char const* cpd_tie_get_name(cpd_tie const* tie)
{
    return tie->s_name;
}






cpd_symbol* cpd_symbol_create(const char* name)
{
    return (cpd_symbol *)gensym(name);
}

char const* cpd_symbol_get_name(cpd_symbol const* symbol)
{
    return symbol->s_name;
}







void cpd_list_init(cpd_list *list, size_t size)
{
    if(size)
    {
        list->vector = (void *)malloc(size * sizeof(t_atom));
        if(list->vector)
        {
            list->size = size;
        }
        else
        {
            list->size = 0;
        }
    }
    else
    {
        list->size      = 0;
        list->vector    = NULL;
    }
}

void cpd_list_clear(cpd_list *list)
{
    if(list->vector && list->size)
    {
        free(list->vector);
    }
    list->vector = NULL;
    list->size   = 0;
}

size_t cpd_list_get_size(cpd_list const* list)
{
    return list->size;
}

cpd_listtype cpd_list_get_type(cpd_list const* list, size_t index)
{
    t_atom const* argv = (t_atom const*)list->vector;
    if((argv+index)->a_type == A_FLOAT)
    {
        return CPD_FLOAT;
    }
    if((argv+index)->a_type == A_SYMBOL)
    {
        return CPD_SYMBOL;
    }
#define LCOV_EXCL_START
    if((argv+index)->a_type == A_POINTER)
    {
        return CPD_POINTER;
    }
#define LCOV_EXCL_STOP
    return CPD_NULL;
}

float cpd_list_get_float(cpd_list const* list, size_t index)
{
    t_atom const* argv = (t_atom const*)list->vector;
    return (argv+index)->a_w.w_float;
}

cpd_symbol* cpd_list_get_symbol(cpd_list const* list, size_t index)
{
    t_atom const* argv = (t_atom const*)list->vector;
    return (argv+index)->a_w.w_symbol;
}

#define LCOV_EXCL_START
cpd_gpointer* cpd_list_get_gpointer(cpd_list const* list, size_t index)
{
    t_atom const* argv = (t_atom const*)list->vector;
    return (argv+index)->a_w.w_gpointer;
}
#define LCOV_EXCL_STOP

void cpd_list_set_float(cpd_list *list, size_t index, float value)
{
    t_atom* argv = (t_atom *)list->vector;
    (argv+index)->a_type = A_FLOAT;
    (argv+index)->a_w.w_float = value;
}

void cpd_list_set_symbol(cpd_list *list, size_t index, cpd_symbol* symbol)
{
    t_atom* argv = (t_atom *)list->vector;
    (argv+index)->a_type = A_SYMBOL;
    (argv+index)->a_w.w_symbol = symbol;
}

#define LCOV_EXCL_START
void cpd_list_set_gpointer(cpd_list *list, size_t index, cpd_gpointer* pointer)
{
    t_atom* argv = (t_atom *)list->vector;
    (argv+index)->a_type = A_POINTER;
    (argv+index)->a_w.w_gpointer = pointer;
}
#define LCOV_EXCL_STOP



