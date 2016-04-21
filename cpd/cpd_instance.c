/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.


#include "cpd_instance.h"
#include "../pd/src/m_pd.h"
#include "../pd/src/s_stuff.h"
#include "../pd/src/m_imp.h"
#include "../pd/src/g_canvas.h"
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>

/*
#ifdef _WIN32
#include <windows.h>
static CRITICAL_SECTION cpd_mutex;
static void cpd_mutex_init()
{
    InitializeCriticalSection(&cpd_mutex, NULL);
}

static void cpd_mutex_destroy()
{
    DeleteCriticalSection(&cpd_mutex);
}

static void cpd_mutex_lock()
{
    EnterCriticalSection(&cpd_mutex);
}

static void cpd_mutex_unlock()
{
    LeaveCriticalSection(&cpd_mutex);
}
#else
#include <pthread.h>
static pthread_mutex_t cpd_mutex;

static void cpd_mutex_init()
{
    pthread_mutex_init(&cpd_mutex, NULL);
}

static void cpd_mutex_destroy()
{
    pthread_mutex_destroy(&cpd_mutex);
}

static void cpd_mutex_lock()
{
    pthread_mutex_lock(&cpd_mutex);
}

static void cpd_mutex_unlock()
{
    pthread_mutex_unlock(&cpd_mutex);
}
#endif
*/

extern cpd_instance*      c_current_instance;
extern t_symbol*          c_sym_pd;
extern t_symbol*          c_sym_dsp;
struct _receiver;

struct _internal
{
    t_pdinstance*           c_intance;
    t_sample*               c_inputs;
    t_sample*               c_outputs;
    int                     c_ninputs;
    int                     c_noutputs;
    int                     cpd_samplerate;
    cpd_hook_post           c_m_normal;
    cpd_hook_post           c_m_log;
    cpd_hook_post           c_m_error;
    cpd_hook_post           c_m_fatal;
    cpd_hook_noteon         c_m_noteon;
    cpd_hook_controlchange  c_m_controlchange;
    cpd_hook_programchange  c_m_programchange;
    cpd_hook_pitchbend      c_m_pitchbend;
    cpd_hook_aftertouch     c_m_aftertouch;
    cpd_hook_polyaftertouch c_m_polyaftertouch;
    cpd_hook_byte           c_m_byte;
    struct _receiver*        c_receiver_list;
};

cpd_instance* cpd_instance_new(size_t size)
{
    cpd_instance* instance = NULL;
    cpd_internal* internal = (cpd_internal *)malloc(sizeof(cpd_internal));
    if(internal)
    {
        internal->c_inputs      = NULL;
        internal->c_outputs     = NULL;
        internal->cpd_samplerate  = 0;
        internal->c_ninputs     = 0;
        internal->c_noutputs    = 0;
        
        
        internal->c_m_normal      = NULL;
        internal->c_m_log       = NULL;
        internal->c_m_error     = NULL;
        internal->c_m_fatal     = NULL;
        
        internal->c_m_noteon        = NULL;
        internal->c_m_controlchange = NULL;
        internal->c_m_programchange = NULL;
        internal->c_m_pitchbend     = NULL;
        internal->c_m_aftertouch    = NULL;
        internal->c_m_polyaftertouch= NULL;
        internal->c_m_byte          = NULL;
        
        internal->c_receiver_list = NULL;
        internal->c_intance = pdinstance_new();
        if(!internal->c_intance)
        {
            free(internal);
            return NULL;
        }
        instance = (cpd_instance *)malloc(size);
        if(instance)
        {
            instance->cpd_internal_ptr = internal;
        }
        else
        {
            pdinstance_free(internal->c_intance);
            free(internal);
            return NULL;
        }
    }
    return instance;
}



static void cpd_instance_set(cpd_instance* instance)
{
    pd_setinstance(instance->cpd_internal_ptr->c_intance);
    sys_soundin     = instance->cpd_internal_ptr->c_inputs;
    sys_soundout    = instance->cpd_internal_ptr->c_outputs;
    sys_inchannels  = instance->cpd_internal_ptr->c_ninputs;
    sys_outchannels = instance->cpd_internal_ptr->c_noutputs;
    sys_dacsr       = instance->cpd_internal_ptr->cpd_samplerate;
    c_current_instance = instance;
}

cpd_patch* cpd_instance_patch_load(cpd_instance* instance, const char* name, const char* path)
{
    int i;
    char* rpath;
    t_canvas* cnv = NULL;
    cpd_instance_set(instance);
    if(name && path)
    {
        cnv = (t_canvas *)glob_evalfile(NULL, gensym(name), gensym(path));
        if(cnv)
        {
            return cnv;
        }
    }
    else if(name)
    {
        
        i = 0;
        while((rpath = namelist_get(sys_searchpath, i)) != NULL)
        {
            cnv = (t_canvas *)glob_evalfile(NULL, gensym(name), gensym(rpath));
            if(cnv)
            {
                return cnv;
            }
            i++;
        }
    }
    return NULL;
}

void cpd_instance_patch_close(cpd_instance* instance, cpd_patch* patch)
{
    cpd_instance_set(instance);
    canvas_free(patch);
}



void cpd_instance_dsp_prepare(cpd_instance* instance,
                              const int nins, const int nouts,
                              const int samplerate, const int nsamples)
{
    cpd_instance_set(instance);
    if(samplerate != instance->cpd_internal_ptr->cpd_samplerate || nins != instance->cpd_internal_ptr->c_ninputs || nouts != instance->cpd_internal_ptr->c_noutputs)
    {
        sys_setchsr(nins, nouts, samplerate);
        instance->cpd_internal_ptr->c_inputs      = sys_soundin;
        instance->cpd_internal_ptr->c_outputs     = sys_soundout;
        instance->cpd_internal_ptr->c_ninputs     = sys_inchannels;
        instance->cpd_internal_ptr->c_noutputs    = sys_outchannels;
        instance->cpd_internal_ptr->cpd_samplerate  = sys_getsr();
    }
    t_atom av;
    av.a_type = A_FLOAT;
    av.a_w.w_float = 1;
    pd_typedmess((t_pd *)c_sym_pd->s_thing, c_sym_dsp, 1, &av);
}

void cpd_instance_dsp_perform(cpd_instance* instance, int nsamples,
                              const int nins, const cpd_sample** inputs,
                              const int nouts, cpd_sample** outputs)
{
    int i, j;
    t_sample *ins = instance->cpd_internal_ptr->c_inputs;
    t_sample *outs = instance->cpd_internal_ptr->c_outputs;
    cpd_instance_set(instance);
    for(i = 0; i < nsamples; i += DEFDACBLKSIZE)
    {
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
}

void cpd_instance_dsp_release(cpd_instance* instance)
{
    cpd_instance_set(instance);
}

int cpd_instance_get_samplerate(cpd_instance* instance)
{
    return instance->cpd_internal_ptr->cpd_samplerate;
}





void cpd_print(const char* s)
{
    int level = 2;
#ifdef DEBUG
    printf("%s", s);
#endif
    if(!c_current_instance)
    {
        return;
    }
    if(strncmp(s, "error:", 6) == 0)
    {
        level = 0;
        s+=5;
    }
    else if(strncmp(s, "verbose(", 8) == 0 && isdigit(s[8]))
    {
        level = atoi(s+8);
        s+=12;
    }
    
    if(level == 0 && c_current_instance->cpd_internal_ptr->c_m_fatal)
    {
        c_current_instance->cpd_internal_ptr->c_m_fatal(c_current_instance, s);
    }
    else if(level == 1 && c_current_instance->cpd_internal_ptr->c_m_error)
    {
        c_current_instance->cpd_internal_ptr->c_m_error(c_current_instance, s);
    }
    else if(level == 2 && c_current_instance->cpd_internal_ptr->c_m_normal)
    {
        c_current_instance->cpd_internal_ptr->c_m_normal(c_current_instance, s);
    }
    else if(c_current_instance->cpd_internal_ptr->c_m_log)
    {
        c_current_instance->cpd_internal_ptr->c_m_log(c_current_instance, s);
    }
}

void cpd_instance_set_hook_console(cpd_instance* instance, cpd_hook_console* consolehook)
{
    instance->cpd_internal_ptr->c_m_normal  = consolehook->m_normal;
    instance->cpd_internal_ptr->c_m_log   = consolehook->m_log;
    instance->cpd_internal_ptr->c_m_error = consolehook->m_error;
    instance->cpd_internal_ptr->c_m_fatal = consolehook->m_fatal;
}


void cpd_instance_post_fatal(cpd_instance* instance, char const* message, ...)
{
    cpd_instance_set(instance);
    char buf[MAXPDSTRING];
    va_list ap;
    va_start(ap, message);
    vsnprintf(buf, MAXPDSTRING-1, message, ap);
    va_end(ap);
    verbose(-3, "%s", buf);
}

void cpd_instance_post_error(cpd_instance* instance, char const* message, ...)
{
    cpd_instance_set(instance);
    char buf[MAXPDSTRING];
    va_list ap;
    va_start(ap, message);
    vsnprintf(buf, MAXPDSTRING-1, message, ap);
    va_end(ap);
    verbose(-2, "%s", buf);
}

void cpd_instance_post_normal(cpd_instance* instance, char const* message, ...)
{
    cpd_instance_set(instance);
    char buf[MAXPDSTRING];
    va_list ap;
    va_start(ap, message);
    vsnprintf(buf, MAXPDSTRING-1, message, ap);
    va_end(ap);
    verbose(-1, "%s", buf);
}

void cpd_instance_post_log(cpd_instance* instance, char const* message, ...)
{
    cpd_instance_set(instance);
    char buf[MAXPDSTRING];
    va_list ap;
    va_start(ap, message);
    vsnprintf(buf, MAXPDSTRING-1, message, ap);
    va_end(ap);
    verbose(0, "%s", buf);
}








static t_class *c_receiver_class;

typedef struct _receiver
{
    t_object            c_obj;
    struct _instance*   c_owner;
    t_symbol*           c_sym;
    cpd_hook_message    c_hook;
    struct _receiver* c_next;
} c_receiver;

static void receiver_bang(c_receiver *x)
{
    if(x->c_hook.m_bang)
    {
        x->c_hook.m_bang(x->c_owner, x->c_sym);
    }
}

static void receiver_float(c_receiver *x, t_float f)
{
    if(x->c_hook.m_float)
    {
        x->c_hook.m_float(x->c_owner, x->c_sym, f);
    }
}

static void receiver_symbol(c_receiver *x, t_symbol *s)
{
    if(x->c_hook.m_symbol)
    {
        x->c_hook.m_symbol(x->c_owner, x->c_sym, s);
    }
}

#define LCOV_EXCL_START
static void receiver_gpointer(c_receiver *x, t_gpointer *gp)
{
    if(x->c_hook.m_gpointer)
    {
        x->c_hook.m_gpointer(x->c_owner, x->c_sym, gp);
    }
}
#define LCOV_EXCL_STOP

struct _ilist
{
    size_t l_n;
    t_atom *l_vec;
};

static void receiver_list(c_receiver *x, t_symbol *s, int argc, t_atom *argv)
{
    if(x->c_hook.m_list)
    {
        struct _ilist l;
        l.l_n = argc;
        l.l_vec = argv;
        x->c_hook.m_list(x->c_owner, x->c_sym, ((cpd_list *)(&l)));
    }
}

static void receiver_anything(c_receiver *x, t_symbol *s, int argc, t_atom *argv)
{
    if(x->c_hook.m_anything)
    {
        struct _ilist l;
        l.l_n = argc;
        l.l_vec = argv;
        x->c_hook.m_anything(x->c_owner, x->c_sym, s, ((cpd_list *)(&l)));
    }
}

static void receiver_free(c_receiver *x)
{
    pd_unbind((t_pd *)x, x->c_sym);
}

extern void receiver_setup(void)
{
    t_class* c = class_new(gensym("c_receiver"),
                           (t_newmethod)NULL,
                           (t_method)receiver_free,
                           sizeof(c_receiver), CLASS_PD, A_DEFSYM, 0);
    if(c)
    {
        class_addbang(c, receiver_bang);
        class_addfloat(c, receiver_float);
        class_addsymbol(c, receiver_symbol);
        class_addpointer(c, receiver_gpointer);
        class_addlist(c, receiver_list);
        class_addanything(c, receiver_anything);
    }
    c_receiver_class = c;
}

static c_receiver* cpd_instance_getreceiver(cpd_instance const* instance, cpd_tie const* tie)
{
    c_receiver* recv = NULL;
    if(instance->cpd_internal_ptr->c_receiver_list)
    {
        recv = instance->cpd_internal_ptr->c_receiver_list;
        while(recv)
        {
            if(recv->c_sym == tie)
            {
                return recv;
            }
            recv = recv->c_next;
        }
    }
    return NULL;
}

void cpd_instance_bind(cpd_instance* instance, cpd_tie* tie, cpd_hook_message const* messagehook)
{
    c_receiver *x = cpd_instance_getreceiver(instance, tie);
    if(x)
    {
        x->c_hook = *messagehook;
    }
    else
    {
        x = (c_receiver *)pd_new(c_receiver_class);
        if(x)
        {
            x->c_sym = tie;
            x->c_owner = instance;
            x->c_hook = *messagehook;
            x->c_next = instance->cpd_internal_ptr->c_receiver_list;
            instance->cpd_internal_ptr->c_receiver_list = x;
            pd_bind((t_pd *)x, x->c_sym);
        }
    }
}

void cpd_instance_unbind(cpd_instance* instance, cpd_tie* tie)
{
    c_receiver *x = cpd_instance_getreceiver(instance, tie), *temp = NULL;
    if(x)
    {
        if(instance->cpd_internal_ptr->c_receiver_list == x)
        {
            instance->cpd_internal_ptr->c_receiver_list = x->c_next;
        }
        else
        {
            temp = instance->cpd_internal_ptr->c_receiver_list;
            while(temp->c_next != x)
            {
                temp = temp->c_next;
            }
            temp->c_next = x->c_next;
        }
        pd_free((t_pd *)x);
    }
}

void cpd_instance_free(cpd_instance* instance)
{
    c_receiver* next = NULL;
    if(c_current_instance == instance)
    {
        c_current_instance = NULL;
    }
    while(instance->cpd_internal_ptr->c_receiver_list)
    {
        next = instance->cpd_internal_ptr->c_receiver_list->c_next;
        pd_free((t_pd *)instance->cpd_internal_ptr->c_receiver_list);
        instance->cpd_internal_ptr->c_receiver_list = next;
    }
    pdinstance_free(instance->cpd_internal_ptr->c_intance);
    instance->cpd_internal_ptr->c_intance = NULL;
    free(instance->cpd_internal_ptr);
    instance->cpd_internal_ptr = NULL;
    free(instance);
}

void cpd_instance_send_bang(cpd_instance* instance, cpd_tie const* tie)
{
    cpd_instance_set(instance);
    t_symbol const* sym = (t_symbol const *)tie;
    if(sym && sym->s_thing)
    {
        pd_bang((t_pd *)sym->s_thing);
    }
}

void cpd_instance_send_float(cpd_instance* instance, cpd_tie const* tie, float value)
{
    cpd_instance_set(instance);
    t_symbol const* sym = (t_symbol const *)tie;
    if(sym && sym->s_thing)
    {
        pd_float((t_pd *)sym->s_thing, value);
    }
}

#define LCOV_EXCL_START
void cpd_instance_send_gpointer(cpd_instance* instance, cpd_tie const* tie, cpd_gpointer const* pointer)
{
    cpd_instance_set(instance);
    t_symbol const* sym = (t_symbol const *)tie;
    if(sym && sym->s_thing)
    {
        pd_pointer((t_pd *)sym->s_thing, (t_gpointer *)pointer);
    }
}
#define LCOV_EXCL_STOP

void cpd_instance_send_symbol(cpd_instance* instance, cpd_tie const* tie, cpd_symbol const* symbol)
{
    cpd_instance_set(instance);
    t_symbol const* sym = (t_symbol const *)tie;
    if(sym && sym->s_thing)
    {
        pd_symbol((t_pd *)sym->s_thing, (t_symbol *)symbol);
    }
}

extern t_atom* cpd_list_get_vec(cpd_list const* list);

void cpd_instance_send_list(cpd_instance* instance, cpd_tie const* tie, cpd_list const* list)
{
    cpd_instance_set(instance);
    if(tie && tie->s_thing)
    {
        pd_list((t_pd *)tie->s_thing, &s_list, (int)cpd_list_get_size(list), cpd_list_get_vec(list));
    }
}

void cpd_instance_send_anything(cpd_instance* instance, cpd_tie const* tie, cpd_symbol const* symbol, cpd_list const* list)
{
    cpd_instance_set(instance);
    if(tie && tie->s_thing)
    {
        pd_typedmess((t_pd *)tie->s_thing, (t_symbol *)symbol, (int)cpd_list_get_size(list), cpd_list_get_vec(list));
    }
}














void cpd_instance_set_hook_midi(cpd_instance* instance, cpd_hook_midi* midihook)
{
    instance->cpd_internal_ptr->c_m_noteon = midihook->m_noteon;
    instance->cpd_internal_ptr->c_m_controlchange = midihook->m_controlchange;
    instance->cpd_internal_ptr->c_m_programchange = midihook->m_programchange;
    instance->cpd_internal_ptr->c_m_pitchbend = midihook->m_pitchbend;
    instance->cpd_internal_ptr->c_m_aftertouch = midihook->m_aftertouch;
    instance->cpd_internal_ptr->c_m_polyaftertouch = midihook->m_polyaftertouch;
    instance->cpd_internal_ptr->c_m_byte = midihook->m_byte;
}

#define CPD_PD_MIDI_CHECK_CHANNEL(channel) if (channel < 0 || channel > 16) return;
#define CPD_PD_MIDI_CHECK_PORT(port) if (port < 0 || port > 0x0fff) return;
#define CPD_PD_MIDI_CHECK_RANGE_7BIT(v) if (v < 0 || v > 0x7f) return;
#define CPD_PD_MIDI_CHECK_RANGE_8BIT(v) if (v < 0 || v > 0xff) return;
#define CPD_PD_MIDI_WRAP_PORT(channel) (channel >> 4)
#define CPD_PD_MIDI_WRAP_CHANNEL(channel) (channel & 0x0f)

void cpd_instance_midi_noteon(cpd_instance* instance, int channel, int pitch, int velocity)
{
    cpd_instance_set(instance);
    CPD_PD_MIDI_CHECK_CHANNEL(channel)
    CPD_PD_MIDI_CHECK_RANGE_7BIT(pitch)
    CPD_PD_MIDI_CHECK_RANGE_8BIT(velocity)
    inmidi_noteon(CPD_PD_MIDI_WRAP_PORT(channel), CPD_PD_MIDI_WRAP_CHANNEL(channel), pitch, velocity);
}

void cpd_instance_midi_controlchange(cpd_instance* instance, int channel, int controller, int value)
{
    cpd_instance_set(instance);
    CPD_PD_MIDI_CHECK_CHANNEL(channel)
    CPD_PD_MIDI_CHECK_RANGE_7BIT(controller)
    CPD_PD_MIDI_CHECK_RANGE_7BIT(value)
    inmidi_controlchange(CPD_PD_MIDI_WRAP_PORT(channel), CPD_PD_MIDI_WRAP_CHANNEL(channel), controller, value);
}

void cpd_instance_midi_programchange(cpd_instance* instance, int channel, int value)
{
    cpd_instance_set(instance);
    CPD_PD_MIDI_CHECK_CHANNEL(channel)
    CPD_PD_MIDI_CHECK_RANGE_7BIT(value)
    inmidi_programchange(CPD_PD_MIDI_WRAP_PORT(channel), CPD_PD_MIDI_WRAP_CHANNEL(channel), value);
}

void cpd_instance_midi_pitchbend(cpd_instance* instance, int channel, int value)
{
    cpd_instance_set(instance);
    CPD_PD_MIDI_CHECK_CHANNEL(channel)
    if (value < -8192 || value > 8191) return;
    inmidi_pitchbend(CPD_PD_MIDI_WRAP_PORT(channel), CPD_PD_MIDI_WRAP_CHANNEL(channel), value + 8192);
}

void cpd_instance_midi_aftertouch(cpd_instance* instance, int channel, int value)
{
    cpd_instance_set(instance);
    CPD_PD_MIDI_CHECK_CHANNEL(channel)
    CPD_PD_MIDI_CHECK_RANGE_7BIT(value)
    inmidi_aftertouch(CPD_PD_MIDI_WRAP_PORT(channel), CPD_PD_MIDI_WRAP_CHANNEL(channel), value);
}

void cpd_instance_midi_polyaftertouch(cpd_instance* instance, int channel, int pitch, int value)
{
    cpd_instance_set(instance);
    CPD_PD_MIDI_CHECK_CHANNEL(channel)
    CPD_PD_MIDI_CHECK_RANGE_7BIT(pitch)
    CPD_PD_MIDI_CHECK_RANGE_7BIT(value)
    inmidi_polyaftertouch(CPD_PD_MIDI_WRAP_PORT(channel), CPD_PD_MIDI_WRAP_CHANNEL(channel), pitch, value);
}

void cpd_instance_midi_byte(cpd_instance* instance, int port, int byte)
{
    cpd_instance_set(instance);
    CPD_PD_MIDI_CHECK_PORT(port)
    CPD_PD_MIDI_CHECK_RANGE_8BIT(byte)
    inmidi_byte(port, byte);
}

#undef CPD_PD_MIDI_CHECK_CHANNEL
#undef CPD_PD_MIDI_CHECK_PORT
#undef CPD_PD_MIDI_CHECK_RANGE_7BIT
#undef CPD_PD_MIDI_CHECK_RANGE_8BIT
#undef CPD_PD_MIDI_WRAP_PORT
#undef CPD_PD_MIDI_WRAP_CHANNEL



#define CPD_PD_MIDI_CLIP(x, low, high) ((x > high) ? high : ((x < low) ? low : x))
#define CPD_PD_MIDI_CLIP4BIT(x) CPD_PD_MIDI_CLIP(x, 0, 0x0f)
#define CPD_PD_MIDI_CLIP7BIT(x) CPD_PD_MIDI_CLIP(x, 0, 0x7f)
#define CPD_PD_MIDI_CLIP8BIT(x) CPD_PD_MIDI_CLIP(x, 0, 0xff)
#define CPD_PD_MIDI_CLIP12BIT(x) CPD_PD_MIDI_CLIP(x, 0, 0x0fff)
#define CPD_PD_MIDI_CLIP14BIT(x) CPD_PD_MIDI_CLIP(x, 0, 0x3fff)
#define CPD_PD_MIDI_CLIPCHANNEL(x) CPD_PD_MIDI_CLIP(x, 0, 15)

void outmidi_noteon(int port, int channel, int pitch, int velo)
{
    if(c_current_instance && c_current_instance->cpd_internal_ptr->c_m_noteon)
    {
        c_current_instance->cpd_internal_ptr->c_m_noteon(c_current_instance, CPD_PD_MIDI_CLIP12BIT(port), CPD_PD_MIDI_CLIPCHANNEL(channel), CPD_PD_MIDI_CLIP7BIT(pitch), CPD_PD_MIDI_CLIP7BIT(velo));
    }
}

void outmidi_controlchange(int port, int channel, int ctl, int value)
{
    if(c_current_instance && c_current_instance->cpd_internal_ptr->c_m_controlchange)
    {
        c_current_instance->cpd_internal_ptr->c_m_controlchange(c_current_instance, CPD_PD_MIDI_CLIP12BIT(port), CPD_PD_MIDI_CLIPCHANNEL(channel), CPD_PD_MIDI_CLIP7BIT(ctl), CPD_PD_MIDI_CLIP7BIT(value));
    }
}

void outmidi_programchange(int port, int channel, int value)
{
    if(c_current_instance && c_current_instance->cpd_internal_ptr->c_m_programchange)
    {
        c_current_instance->cpd_internal_ptr->c_m_programchange(c_current_instance, CPD_PD_MIDI_CLIP12BIT(port), CPD_PD_MIDI_CLIPCHANNEL(channel), CPD_PD_MIDI_CLIP7BIT(value));
    }
}

void outmidi_pitchbend(int port, int channel, int value)
{
    if(c_current_instance && c_current_instance->cpd_internal_ptr->c_m_pitchbend)
    {
        c_current_instance->cpd_internal_ptr->c_m_pitchbend(c_current_instance, CPD_PD_MIDI_CLIP12BIT(port), CPD_PD_MIDI_CLIPCHANNEL(channel), CPD_PD_MIDI_CLIP14BIT(value) - 8192);
    }
}

void outmidi_aftertouch(int port, int channel, int value)
{
    if(c_current_instance && c_current_instance->cpd_internal_ptr->c_m_aftertouch)
    {
        c_current_instance->cpd_internal_ptr->c_m_aftertouch(c_current_instance, CPD_PD_MIDI_CLIP12BIT(port), CPD_PD_MIDI_CLIPCHANNEL(channel), CPD_PD_MIDI_CLIP7BIT(value));
    }
}

void outmidi_polyaftertouch(int port, int channel, int pitch, int value)
{
    if(c_current_instance && c_current_instance->cpd_internal_ptr->c_m_polyaftertouch)
    {
        c_current_instance->cpd_internal_ptr->c_m_polyaftertouch(c_current_instance, CPD_PD_MIDI_CLIP12BIT(port), CPD_PD_MIDI_CLIPCHANNEL(channel), CPD_PD_MIDI_CLIP7BIT(pitch), CPD_PD_MIDI_CLIP7BIT(value));
    }
}

void outmidi_byte(int port, int value)
{
    if(c_current_instance && c_current_instance->cpd_internal_ptr->c_m_byte)
    {
        c_current_instance->cpd_internal_ptr->c_m_byte(c_current_instance, CPD_PD_MIDI_CLIP12BIT(port), CPD_PD_MIDI_CLIP8BIT(value));
    }
}

#undef CPD_PD_MIDI_CLIP
#undef CPD_PD_MIDI_CLIP4BIT
#undef CPD_PD_MIDI_CLIP7BIT
#undef CPD_PD_MIDI_CLIP8BIT
#undef CPD_PD_MIDI_CLIP12BIT
#undef CPD_PD_MIDI_CLIP14BIT
#undef CPD_PD_MIDI_CLIPCHANNEL



