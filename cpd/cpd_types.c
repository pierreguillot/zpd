/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.


#include "cpd_types.h"
#include "../pd/src/m_pd.h"
#include <stdlib.h>

struct _list
{
    size_t l_n;
    t_atom *l_vec;
};

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







cpd_list* cpd_list_create(size_t size)
{
    cpd_list *x   = (cpd_list *)malloc(sizeof(cpd_list));
    if(x)
    {
        if(size)
        {
            x->l_vec = (t_atom *)malloc(size * sizeof(t_atom));
            if(x->l_vec)
            {
                x->l_n = size;
            }
            else
            {
                x->l_n = 0;
            }
        }
        else
        {
            x->l_n      = 0;
            x->l_vec    = NULL;
        }
    }
    return x;
}

void cpd_list_free(cpd_list *list)
{
    if(list->l_vec && list->l_n)
    {
        free(list->l_vec);
    }
    list->l_vec = NULL;
    list->l_n   = 0;
    free(list);
}

size_t cpd_list_get_size(cpd_list const* list)
{
    return list->l_n;
}

cpd_listtype cpd_list_get_type(cpd_list const* list, size_t index)
{
    if((list->l_vec+index)->a_type == A_FLOAT)
    {
        return CPD_FLOAT;
    }
    if((list->l_vec+index)->a_type == A_SYMBOL)
    {
        return CPD_SYMBOL;
    }
#define LCOV_EXCL_START
    if((list->l_vec+index)->a_type == A_POINTER)
    {
        return CPD_POINTER;
    }
#define LCOV_EXCL_STOP
    return CPD_NULL;
}

float cpd_list_get_float(cpd_list const* list, size_t index)
{
    return (list->l_vec+index)->a_w.w_float;
}

cpd_symbol* cpd_list_get_symbol(cpd_list const* list, size_t index)
{
    return (list->l_vec+index)->a_w.w_symbol;
}

#define LCOV_EXCL_START
cpd_gpointer* cpd_list_get_gpointer(cpd_list const* list, size_t index)
{
    return (list->l_vec+index)->a_w.w_gpointer;
}
#define LCOV_EXCL_STOP

void cpd_list_set_float(cpd_list *list, size_t index, float value)
{
    (list->l_vec+index)->a_type = A_FLOAT;
    (list->l_vec+index)->a_w.w_float = value;
}

void cpd_list_set_symbol(cpd_list *list, size_t index, cpd_symbol* symbol)
{
    (list->l_vec+index)->a_type = A_SYMBOL;
    (list->l_vec+index)->a_w.w_symbol = symbol;
}

#define LCOV_EXCL_START
void cpd_list_set_gpointer(cpd_list *list, size_t index, cpd_gpointer* pointer)
{
    (list->l_vec+index)->a_type = A_POINTER;
    (list->l_vec+index)->a_w.w_gpointer = pointer;
}
#define LCOV_EXCL_STOP


t_atom* cpd_list_get_vec(cpd_list const* list)
{
    return list->l_vec;
}


