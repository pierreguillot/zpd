/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.


#include "cpd_dsp.h"
#include "../pd/src/m_pd.h"
#include "../pd/src/s_stuff.h"
#include <string.h>

static t_symbol* c_sym_dsp;
static t_symbol* c_sym_pd;

extern void cpd_instance_lock(cpd_instance* instance);
extern void cpd_instance_unlock(cpd_instance *instance);
extern void cpd_midi_manager_perform(struct cpd_midi_manager* instance);
extern void cpd_message_manager_perform(struct cpd_message_manager* manager);

struct cpd_dsp_manager
{
    t_sample*       c_inputs;
    t_sample*       c_outputs;
    int             c_samplerate;
    int             c_ninputs;
    int             c_noutputs;
};


extern void cpd_dsp_manager_init(struct cpd_dsp_manager* manager)
{
    static int initialized = 0;
    if(!initialized)
    {
        c_sym_dsp = gensym("dsp");
        c_sym_pd  = gensym("pd");
        initialized = 1;
    }
    manager->c_inputs       = NULL;
    manager->c_outputs      = NULL;
    manager->c_samplerate   = 0;
    manager->c_ninputs      = 0;
    manager->c_noutputs     = 0;
}

extern void cpd_dsp_manager_clear(struct cpd_dsp_manager* manager)
{
    manager->c_inputs       = NULL;
    manager->c_outputs      = NULL;
    manager->c_samplerate   = 0;
    manager->c_ninputs      = 0;
    manager->c_noutputs     = 0;
}

void cpd_instance_dsp_prepare(cpd_instance* instance, const int nins, const int nouts, const int samplerate, const int nsamples)
{
    cpd_instance_lock(instance);
    sys_soundin     = instance->c_dsp->c_inputs;
    sys_soundout    = instance->c_dsp->c_outputs;
    sys_inchannels  = instance->c_dsp->c_ninputs;
    sys_outchannels = instance->c_dsp->c_noutputs;
    sys_dacsr       = instance->c_dsp->c_samplerate;
    
    if(samplerate != instance->c_dsp->c_samplerate
       || nins != instance->c_dsp->c_ninputs
       || nouts != instance->c_dsp->c_noutputs)
    {
        sys_setchsr(nins, nouts, samplerate);
        instance->c_dsp->c_inputs      = sys_soundin;
        instance->c_dsp->c_outputs     = sys_soundout;
        instance->c_dsp->c_ninputs     = sys_inchannels;
        instance->c_dsp->c_noutputs    = sys_outchannels;
        instance->c_dsp->c_samplerate  = sys_getsr();
    }
    t_atom av;
    av.a_type = A_FLOAT;
    av.a_w.w_float = 1;
    pd_typedmess((t_pd *)c_sym_pd->s_thing, c_sym_dsp, 1, &av);
    cpd_instance_unlock(instance);
}



void cpd_instance_dsp_perform(cpd_instance* instance, int nsamples, const int nins, const cpd_sample** inputs, const int nouts, cpd_sample** outputs)
{
    int i, j;
    t_sample *ins = instance->c_dsp->c_inputs;
    t_sample *outs = instance->c_dsp->c_outputs;
    cpd_instance_lock(instance);
    sys_soundin     = instance->c_dsp->c_inputs;
    sys_soundout    = instance->c_dsp->c_outputs;
    sys_inchannels  = instance->c_dsp->c_ninputs;
    sys_outchannels = instance->c_dsp->c_noutputs;
    sys_dacsr       = instance->c_dsp->c_samplerate;
    
    for(i = 0; i < nsamples; i += DEFDACBLKSIZE)
    {
        cpd_message_manager_perform(instance->c_message);
        cpd_midi_manager_perform(instance->c_midi);
        for(j = 0; j < nins; j++)
        {
            memcpy(ins+j*DEFDACBLKSIZE, inputs[j]+i, DEFDACBLKSIZE * sizeof(t_sample));
        }
        memset(outs, 0, DEFDACBLKSIZE * sizeof(t_sample) * nouts);
        sched_tick();
        for(j = 0; j < nouts; j++)
        {
            memcpy(outputs[j]+i, outs+j*DEFDACBLKSIZE, DEFDACBLKSIZE * sizeof(t_sample));
        }
    }
    cpd_instance_unlock(instance);
}

void cpd_instance_dsp_release(cpd_instance* instance)
{
    
}

int cpd_instance_get_samplerate(cpd_instance* instance)
{
    return instance->c_dsp->c_samplerate;
}

