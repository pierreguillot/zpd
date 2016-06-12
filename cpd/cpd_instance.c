/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.


#include "cpd_instance.h"
#include "cpd_midi.h"
#include "cpd_message.h"

#include "../pd/src/m_pd.h"
#include "../pd/src/s_stuff.h"
#include "../pd/src/m_imp.h"
#include "../pd/src/g_canvas.h"
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>

extern void cpd_lock();
extern void cpd_unlock();

extern void cpd_dsp_manager_init(cpd_instance* instance);
extern void cpd_message_manager_init(cpd_instance* instance, size_t size);
extern void cpd_midi_manager_init(cpd_instance* instance, size_t size);
extern void cpd_post_manager_init(cpd_instance* instance);

extern void cpd_dsp_manager_clear(cpd_instance* instance);
extern void cpd_message_manager_clear(cpd_instance* instance);
extern void cpd_midi_manager_clear(cpd_instance* instance);
extern void cpd_post_manager_clear(cpd_instance* instance);

cpd_instance* c_current_instance = NULL;

cpd_instance* cpd_instance_new(size_t size)
{
    cpd_instance* instance = (cpd_instance *)malloc(size);
    if(instance)
    {
        instance->c_internal = pdinstance_new();
        cpd_dsp_manager_init(instance);
        cpd_message_manager_init(instance, 512);
        cpd_midi_manager_init(instance, 512);
        cpd_post_manager_init(instance);
    }
    return instance;
}

void cpd_instance_free(cpd_instance* instance)
{
    cpd_lock();
    if(c_current_instance == instance)
    {
        c_current_instance = NULL;
    }
    cpd_unlock();
    cpd_midi_manager_clear(instance);
    cpd_message_manager_clear(instance);
    cpd_dsp_manager_clear(instance);
    cpd_post_manager_clear(instance);
    free(instance);
}

extern void cpd_instance_lock(cpd_instance* instance)
{
    cpd_lock();
    c_current_instance = instance;
    pd_setinstance(instance->c_internal);
}

extern void cpd_instance_unlock(cpd_instance* instance)
{
    cpd_unlock();
}




































