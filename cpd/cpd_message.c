/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.


#include "cpd_message.h"
#include "cpd_mutex.h"
#include "../pd/src/m_pd.h"
#include "../pd/src/s_stuff.h"
#include <stdlib.h>

extern cpd_instance* c_current_instance;

typedef struct cpd_receiver
{
    t_object                c_obj;
    cpd_instance*           c_owner;
    t_symbol*               c_sym;
    cpd_message_hook        c_hook;
    struct cpd_receiver*    c_next;
} cpd_receiver;

struct cpd_message_manager
{
    cpd_instance*       c_instance;
    cpd_message_hook    c_hook;
    cpd_message*        c_buffer;
    size_t              c_size;
    size_t              c_pos;
    cpd_mutex           c_mutex;
    cpd_receiver*       c_receivers;
};



// ==================================================================================== //
//                                      ALLOCATION                                      //
// ==================================================================================== //

static char cpd_message_manager_alloc(struct cpd_message_manager* manager, size_t newsize)
{
    cpd_message* temp;
    if(newsize <= manager->c_size)
    {
        return 1;
    }
    if(manager->c_buffer && manager->c_size)
    {
        temp = (cpd_message *)realloc(manager->c_buffer, sizeof(cpd_message) * newsize);
        if(temp)
        {
            manager->c_buffer = temp;
            manager->c_size   = newsize;
            return 1;
        }
    }
    else
    {
        manager->c_buffer = (cpd_message *)malloc(sizeof(cpd_message) * newsize);
        if(manager->c_buffer)
        {
            manager->c_size = newsize;
            return 1;
        }
    }
    return 0;
}

// ==================================================================================== //
//                                      INTERNAL                                        //
// ==================================================================================== //

static t_class *cpd_receiver_class;

static void receiver_anything(cpd_receiver *x, t_symbol *s, int argc, t_atom *argv)
{
    if(x->c_hook)
    {
        x->c_hook(x->c_owner, (cpd_message){x->c_sym, s, (cpd_list){(size_t)argc, (void *)argv}});
    }
}

static void receiver_free(cpd_receiver *x)
{
    pd_unbind((t_pd *)x, x->c_sym);
}

extern void cpd_message_manager_init(cpd_instance* instance, size_t size)
{
    static t_class* c = NULL;
    if(!c)
    {
        c = class_new(gensym("cpd_receiver"),
                  (t_newmethod)NULL,
                  (t_method)receiver_free,
                  sizeof(cpd_receiver), CLASS_PD, A_DEFSYM, 0);
        if(c)
        {
            class_addanything(c, receiver_anything);
        }
        cpd_receiver_class = c;
    }
    
    instance->c_message = (struct cpd_message_manager *)malloc(sizeof(struct cpd_message_manager));
    if(instance->c_message)
    {
        instance->c_message->c_hook     = NULL;
        instance->c_message->c_buffer   = NULL;
        instance->c_message->c_size     = 0;
        instance->c_message->c_pos      = 0;
        instance->c_message->c_receivers= NULL;
        cpd_message_manager_alloc(instance->c_message, size);
        cpd_mutex_init(&(instance->c_message->c_mutex));
    }
}

extern void cpd_message_manager_clear(cpd_instance* instance)
{
    cpd_receiver* next = NULL;
    while(instance->c_message->c_receivers)
    {
        next = instance->c_message->c_receivers->c_next;
        pd_free((t_pd *)instance->c_message->c_receivers);
        instance->c_message->c_receivers = next;
    }
    if(instance->c_message->c_buffer && instance->c_message->c_size)
    {
        free(instance->c_message->c_buffer);
    }
    instance->c_message->c_buffer = NULL;
    instance->c_message->c_size = 0;
    instance->c_message->c_pos  = 0;
    cpd_mutex_destroy(&(instance->c_message->c_mutex));
    free(instance->c_message);
}

extern void cpd_message_manager_perform(struct cpd_message_manager* manager)
{
    size_t i;
    cpd_message const* buffer;
    cpd_mutex_lock(&(manager->c_mutex));
    buffer = manager->c_buffer;
    for(i = 0; i < manager->c_pos; ++i)
    {
        if(buffer[i].tie->s_thing)
        {
            pd_typedmess((t_pd *)(buffer[i].tie->s_thing), buffer[i].selector, 0, NULL);
        }
    }
    manager->c_pos = 0;
    cpd_mutex_unlock(&(manager->c_mutex));
}



// ==================================================================================== //
//                                      INTERFACE                                       //
// ==================================================================================== //

static cpd_receiver* cpd_message_manager_getreceiver(struct cpd_message_manager* manager, cpd_tie const* tie)
{
    cpd_receiver* recv = manager->c_receivers;
    while(recv)
    {
        if(recv->c_sym == tie)
        {
            return recv;
        }
        recv = recv->c_next;
    }
    return NULL;
}

void cpd_instance_bind(cpd_instance* instance, cpd_tie* tie, cpd_message_hook messagehook)
{
    cpd_receiver *x = NULL;
    struct cpd_message_manager* manager = instance->c_message;
    if(manager)
    {
        x = cpd_message_manager_getreceiver(manager, tie);
        if(x)
        {
            x->c_hook = messagehook;
        }
        else
        {
            x = (cpd_receiver *)pd_new(cpd_receiver_class);
            if(x)
            {
                x->c_sym = tie;
                x->c_owner = instance;
                x->c_hook = messagehook;
                x->c_next = manager->c_receivers;
                manager->c_receivers = x;
                pd_bind((t_pd *)x, x->c_sym);
            }
        }
    }
}

void cpd_instance_unbind(cpd_instance* instance, cpd_tie* tie)
{
    cpd_receiver *temp  = NULL;
    cpd_receiver *x     = NULL;
    struct cpd_message_manager* manager = instance->c_message;
    if(manager)
    {
        x = cpd_message_manager_getreceiver(manager, tie);
        if(x)
        {
            if(manager->c_receivers == x)
            {
                manager->c_receivers = x->c_next;
            }
            else
            {
                temp = manager->c_receivers;
                while(temp->c_next != x)
                {
                    temp = temp->c_next;
                }
                temp->c_next = x->c_next;
            }
            pd_free((t_pd *)x);
        }
    }
    
}

void cpd_instance_message_send(cpd_instance* instance, cpd_message event)
{
    struct cpd_message_manager* manager = instance->c_message;
    if(manager)
    {
        cpd_mutex_lock(&(manager->c_mutex));
        if(cpd_message_manager_alloc(manager, manager->c_pos+1))
        {
            manager->c_buffer[manager->c_pos++] = event;
        }
        cpd_mutex_unlock(&(manager->c_mutex));
    }
}






