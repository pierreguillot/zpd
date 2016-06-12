/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.


#include "cpd_midi.h"
#include "cpd_mutex.h"
#include "../pd/src/m_pd.h"
#include "../pd/src/s_stuff.h"
#include <stdlib.h>


extern cpd_instance* c_current_instance;

struct cpd_midi_manager
{
    cpd_midi_hook   c_hook;
    cpd_midi_event* c_buffer;
    size_t          c_size;
    size_t          c_pos;
    cpd_mutex       c_mutex;
};

// ==================================================================================== //
//                                      ALLOCATION                                      //
// ==================================================================================== //

static char cpd_midi_manager_alloc(struct cpd_midi_manager* manager, size_t newsize)
{
    cpd_midi_event* temp;
    if(newsize <= manager->c_size)
    {
        return 1;
    }
    if(manager->c_buffer && manager->c_size)
    {
        temp = (cpd_midi_event *)realloc(manager->c_buffer, sizeof(cpd_midi_event) * newsize);
        if(temp)
        {
            manager->c_buffer = temp;
            manager->c_size   = newsize;
            return 1;
        }
    }
    else
    {
        manager->c_buffer = (cpd_midi_event *)malloc(sizeof(cpd_midi_event) * newsize);
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

extern void cpd_midi_manager_init(cpd_instance* instance, size_t size)
{
    instance->c_midi = (struct cpd_midi_manager *)malloc(sizeof(struct cpd_midi_manager));
    if(instance->c_midi)
    {
        instance->c_midi->c_hook     = NULL;
        instance->c_midi->c_buffer   = NULL;
        instance->c_midi->c_size     = 0;
        instance->c_midi->c_pos      = 0;
        cpd_midi_manager_alloc(instance->c_midi, size);
        cpd_mutex_init(&(instance->c_midi->c_mutex));
    }
}

extern void cpd_midi_manager_clear(cpd_instance* instance)
{
    if(instance->c_midi->c_buffer && instance->c_midi->c_size)
    {
        free(instance->c_midi->c_buffer);
    }
    instance->c_midi->c_buffer = NULL;
    instance->c_midi->c_size = 0;
    instance->c_midi->c_pos  = 0;
    cpd_mutex_destroy(&(instance->c_midi->c_mutex));
    free(instance->c_midi);
}

extern void cpd_midi_manager_perform(struct cpd_midi_manager* manager)
{
    size_t i;
    cpd_midi_event const* buffer;
    cpd_mutex_lock(&(manager->c_mutex));
    buffer = manager->c_buffer;
    for(i = 0; i < manager->c_pos; ++i)
    {
        if(buffer[i].type == CPD_MIDI_NOTE)
        {
            inmidi_noteon(buffer[i].data1 >> 4, buffer[i].data1, buffer[i].data2, buffer[i].data3);
        }
        else if(buffer[i].type == CPD_MIDI_CTRL)
        {
            inmidi_controlchange(buffer[i].data1 >> 4, buffer[i].data1, buffer[i].data2, buffer[i].data3);
        }
        else if(buffer[i].type == CPD_MIDI_PGRM)
        {
            inmidi_programchange(buffer[i].data1 >> 4, buffer[i].data1, buffer[i].data2);
        }
        else if(buffer[i].type == CPD_MIDI_BEND)
        {
            inmidi_pitchbend(buffer[i].data1 >> 4, buffer[i].data1, buffer[i].data2);
        }
        else if(buffer[i].type == CPD_MIDI_ATOUCH)
        {
            inmidi_aftertouch(buffer[i].data1 >> 4, buffer[i].data1, buffer[i].data3);
        }
        else if(buffer[i].type == CPD_MIDI_PATOUCH)
        {
            inmidi_polyaftertouch(buffer[i].data1 >> 4, buffer[i].data1, buffer[i].data2, buffer[i].data3);
        }
        else if(buffer[i].type == CPD_MIDI_BYTE)
        {
            inmidi_byte(buffer[i].data1 >> 4, buffer[i].data3);
        }
    }
    manager->c_pos = 0;
    cpd_mutex_unlock(&(manager->c_mutex));
}



// ==================================================================================== //
//                                      INTERFACE                                       //
// ==================================================================================== //

void cpd_instance_midi_sethook(cpd_instance* instance, cpd_midi_hook midihook)
{
    struct cpd_midi_manager* manager = instance->c_midi;
    if(manager)
    {
        manager->c_hook = midihook;
    }
}

void cpd_instance_midi_send(cpd_instance* instance, cpd_midi_event event)
{
    struct cpd_midi_manager* manager =instance->c_midi;
    if(manager)
    {
        cpd_mutex_lock(&(manager->c_mutex));
        if(cpd_midi_manager_alloc(manager, manager->c_pos+1))
        {
            manager->c_buffer[manager->c_pos++] = event;
        }
        cpd_mutex_unlock(&(manager->c_mutex));
    }
}


// ==================================================================================== //
//                                      PURE DATA                                       //
// ==================================================================================== //

void outmidi_noteon(int port, int channel, int pitch, int velocity)
{
    cpd_instance* instance = c_current_instance;
    if(instance)
    {
        if(instance->c_midi && instance->c_midi->c_hook)
        {
            instance->c_midi->c_hook(c_current_instance, (cpd_midi_event){CPD_MIDI_NOTE, channel, pitch, velocity});
        }
    }
}

void outmidi_controlchange(int port, int channel, int contoller, int value)
{
    cpd_instance* instance = c_current_instance;
    if(instance)
    {
        if(instance->c_midi && instance->c_midi->c_hook)
        {
            instance->c_midi->c_hook(instance, (cpd_midi_event){CPD_MIDI_CTRL, channel, contoller, value});
        }
    }
}

void outmidi_programchange(int port, int channel, int program)
{
    cpd_instance* instance = c_current_instance;
    if(instance)
    {
        if(instance->c_midi && instance->c_midi->c_hook)
        {
            instance->c_midi->c_hook(instance, (cpd_midi_event){CPD_MIDI_PGRM, channel, program, 0});
        }
    }
}

void outmidi_pitchbend(int port, int channel, int value)
{
    cpd_instance* instance = c_current_instance;
    if(instance)
    {
        if(instance->c_midi && instance->c_midi->c_hook)
        {
            instance->c_midi->c_hook(instance, (cpd_midi_event){CPD_MIDI_BEND, channel, value - 8192, 0});
        }
    }
}

void outmidi_aftertouch(int port, int channel, int value)
{
    cpd_instance* instance = c_current_instance;
    if(instance)
    {
        if(instance->c_midi && instance->c_midi->c_hook)
        {
            instance->c_midi->c_hook(instance, (cpd_midi_event){CPD_MIDI_ATOUCH, channel, 0, value});
        }
    }
}

void outmidi_polyaftertouch(int port, int channel, int pitch, int value)
{
    cpd_instance* instance = c_current_instance;
    if(instance)
    {
        if(instance->c_midi && instance->c_midi->c_hook)
        {
            instance->c_midi->c_hook(instance, (cpd_midi_event){CPD_MIDI_PATOUCH, channel,  pitch, value});
        }
    }
}

void outmidi_byte(int port, int value)
{
    cpd_instance* instance = c_current_instance;
    if(instance)
    {
        if(instance->c_midi && instance->c_midi->c_hook)
        {
            instance->c_midi->c_hook(instance, (cpd_midi_event){CPD_MIDI_PATOUCH, port, 0, value});
        }
    }
}

