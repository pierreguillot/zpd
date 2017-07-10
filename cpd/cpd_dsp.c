/*
// Copyright (c) 2015-2016 Pierre Guillot.
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
#include <stdlib.h>

extern t_symbol* c_sym_dsp;
extern t_symbol* c_sym_pd;

extern void cpd_instance_lock(cpd_instance* instance);
extern void cpd_instance_unlock(cpd_instance *instance);
extern void cpd_midi_manager_perform(struct cpd_midi_manager* instance);
extern void cpd_message_manager_perform(struct cpd_message_manager* manager);


// ==================================================================================== //
//                                      INTERFACE                                       //
// ==================================================================================== //

void cpd_instance_dsp_prepare(cpd_instance* instance, const int nins, const int nouts, const int samplerate, const int nsamples)
{
    cpd_instance_lock(instance);
    if(samplerate != sys_getsr() || nins != sys_get_inchannels() || nouts != sys_get_outchannels())
    {
        sys_setchsr(nins, nouts, samplerate);
    }
    t_atom av;
    av.a_type = A_FLOAT;
    av.a_w.w_float = 1;
    pd_typedmess(gensym("pd")->s_thing, gensym("dsp"), 1, &av);
    cpd_instance_unlock(instance);
}



void cpd_instance_dsp_perform(cpd_instance* instance, int nsamples, const int nins, const cpd_sample** inputs, const int nouts, cpd_sample** outputs)
{
    int i, j;
    t_sample *ins  = NULL;
    t_sample *outs = NULL;
    cpd_instance_lock(instance);
    ins = get_sys_soundin();
    outs = get_sys_soundout();
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

int cpd_instance_get_samplerate(cpd_instance* instance)
{
    cpd_instance_lock(instance);
    int sr = sys_getsr();
    cpd_instance_unlock(instance);
    return sr;
}

