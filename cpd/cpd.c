/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#include <ctype.h>
#include <stdarg.h>
#include <assert.h>

#include "cpd.h"
#include "../pd/src/m_pd.h"
#include "../pd/src/g_canvas.h"
#include "../pd/src/s_stuff.h"
#include "../pd/src/m_imp.h"
#include "../pd/src/g_all_guis.h"


EXTERN void pd_init(void);

void bob_tilde_setup();
void bonk_tilde_setup();
void choice_setup();
void fiddle_tilde_setup();
void loop_tilde_setup();
void lrshift_tilde_setup();
void pique_setup();
void sigmund_tilde_setup();
void stdout_setup();
int sys_startgui(const char *libdir);

#define LCOV_EXCL_START
void sys_get_midi_apis(char *buf) {}
void sys_listmididevs(void) {}
void sys_get_midi_params(int *pnmidiindev, int *pmidiindev,int *pnmidioutdev, int *pmidioutdev) {}
void sys_open_midi(int nmidiindev, int *midiindev, int nmidioutdev, int *midioutdev, int enable) {}
void sys_close_midi() {}
void sys_reopen_midi(void) {}
void sys_initmidiqueue(void) {}
void sys_pollmidiqueue(void) {}
void sys_setmiditimediff(double inbuftime, double outbuftime) {}
void glob_midi_setapi(void *dummy, t_floatarg f) {}
void glob_midi_properties(t_pd *dummy, t_floatarg flongform) {}
void glob_midi_dialog(t_pd *dummy, t_symbol *s, int argc, t_atom *argv) {}
int sys_mididevnametonumber(int output, const char *name) { return 0; }
void sys_mididevnumbertoname(int output, int devno, char *name, int namesize) {}
#define LCOV_EXCL_STOP

static t_class *c_receiver_class;

typedef struct _receiver
{
    t_object            c_obj;
    struct _instance*   c_owner;
    t_symbol*           c_sym;
    cpd_hook_bang         c_m_bang;
    cpd_hook_float        c_m_float;
    cpd_hook_symbol       c_m_symbol;
    cpd_hook_gpointer     c_m_gpointer;
    cpd_hook_list         c_m_list;
    cpd_hook_anything     c_m_anything;
    struct _receiver* c_next;
} c_receiver;

static void receiver_bang(c_receiver *x)
{
    if(x->c_m_bang)
    {
        x->c_m_bang(x->c_owner, x->c_sym);
    }
}

static void receiver_float(c_receiver *x, t_float f)
{
    if(x->c_m_float)
    {
        x->c_m_float(x->c_owner, x->c_sym, f);
    }
}

static void receiver_symbol(c_receiver *x, t_symbol *s)
{
    if(x->c_m_symbol)
    {
        x->c_m_symbol(x->c_owner, x->c_sym, s);
    }
}

#define LCOV_EXCL_START
static void receiver_gpointer(c_receiver *x, t_gpointer *gp)
{
    if(x->c_m_gpointer)
    {
        x->c_m_gpointer(x->c_owner, x->c_sym, gp);
    }
}
#define LCOV_EXCL_STOP

struct _list
{
    size_t l_n;
    t_atom *l_vec;
};

static void receiver_list(c_receiver *x, t_symbol *s, int argc, t_atom *argv)
{
    if(x->c_m_list)
    {
        cpd_list b;
        b.l_n = argc;
        b.l_vec = argv;
        x->c_m_list(x->c_owner, x->c_sym, &b);
    }
}

static void receiver_anything(c_receiver *x, t_symbol *s, int argc, t_atom *argv)
{
    if(x->c_m_anything)
    {
        cpd_list b;
        b.l_n = argc;
        b.l_vec = argv;
        x->c_m_anything(x->c_owner, x->c_sym, s, &b);
    }
}

static void receiver_free(c_receiver *x)
{
    pd_unbind((t_pd *)x, x->c_sym);
}

void receiver_setup(void)
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





struct _internal
{
    t_pdinstance*           c_intance;
    t_sample*               c_inputs;
    t_sample*               c_outputs;
    int                     c_ninputs;
    int                     c_noutputs;
    int                     cpd_samplerate;
    cpd_hook_post            c_m_normal;
    cpd_hook_post            c_m_log;
    cpd_hook_post            c_m_error;
    cpd_hook_post            c_m_fatal;
    cpd_hook_noteon           c_m_noteon;
    cpd_hook_controlchange    c_m_controlchange;
    cpd_hook_programchange    c_m_programchange;
    cpd_hook_pitchbend        c_m_pitchbend;
    cpd_hook_aftertouch       c_m_aftertouch;
    cpd_hook_polyaftertouch   c_m_polyaftertouch;
    cpd_hook_byte             c_m_byte;
    c_receiver*             c_receiver_list;
};

static t_sample*        cpd_sample_ins        = NULL;
static t_sample*        cpd_sample_outs       = NULL;
static t_pdinstance*    c_first_instance    = NULL;
static cpd_instance*      c_current_instance  = NULL;
static t_symbol*        c_sym_pd            = NULL;
static t_symbol*        c_sym_dsp           = NULL;
static cpd_symbol*        c_sym_bng           = NULL;
static cpd_symbol*        c_sym_hsl           = NULL;
static cpd_symbol*        c_sym_vsl           = NULL;
static cpd_symbol*        c_sym_tgl           = NULL;
static cpd_symbol*        c_sym_nbx           = NULL;
static cpd_symbol*        c_sym_vradio        = NULL;
static cpd_symbol*        c_sym_hradio        = NULL;
static cpd_symbol*        c_sym_vu            = NULL;
static cpd_symbol*        c_sym_cnv           = NULL;
static cpd_symbol*        c_sym_empty         = NULL;

static void cpd_print(const char* s);

void cpd_init()
{
    int devices = 0;
    int ioputs  = 2;
    static char initialized = 0;
    assert("Pure Data is already initialized." && !initialized);
    if(!initialized)
    {
        signal(SIGFPE, SIG_IGN);
        sys_printhook = NULL;
        sys_soundin = NULL;
        sys_soundout = NULL;
        sys_schedblocksize = DEFDACBLKSIZE;
        sys_externalschedlib = 0;
        sys_printtostderr = 0;
        sys_usestdpath = 0;
        sys_debuglevel = 1;
        sys_verbose = 4;
        sys_noloadbang = 0;
        sys_nogui = 1;
        sys_hipriority = 0;
        sys_nmidiin = 0;
        sys_nmidiout = 0;
        sys_init_fdpoll();
        sys_startgui(NULL);
        pd_init();
        sys_set_audio_api(API_DUMMY);
        sys_searchpath = NULL;
        sys_set_audio_settings(1, &devices, 1, &ioputs, 1, &devices, 1, &ioputs, 44100, -1, 1, DEFDACBLKSIZE);
        sched_set_using_audio(SCHED_AUDIO_CALLBACK);
        sys_reopen_audio();
        cpd_sample_ins    = sys_soundin;
        cpd_sample_outs   = sys_soundout;
        c_first_instance= pd_this;
        sys_soundin         = NULL;
        sys_soundout        = NULL;
        c_current_instance  = NULL;
     
        c_sym_pd            = gensym("pd");
        c_sym_dsp           = gensym("dsp");
        c_sym_bng           = gensym("bng");
        c_sym_hsl           = gensym("hsl");
        c_sym_vsl           = gensym("vsl");
        c_sym_tgl           = gensym("tgl");
        c_sym_nbx           = gensym("nbx");
        c_sym_vradio        = gensym("vradio");
        c_sym_hradio        = gensym("hradio");
        c_sym_vu            = gensym("vu");
        c_sym_cnv           = gensym("cnv");
        c_sym_empty         = gensym("empty");
        
        bob_tilde_setup();
        bonk_tilde_setup();
        choice_setup();
        fiddle_tilde_setup();
        loop_tilde_setup();
        lrshift_tilde_setup();
        pique_setup();
        sigmund_tilde_setup();
        stdout_setup();
        receiver_setup();
        
        sys_printhook = (t_printhook)(cpd_print);
        initialized = 1;
    }
}

void cpd_clear()
{
    if(cpd_sample_ins)
    {
        freebytes(cpd_sample_ins, (sys_inchannels ? sys_inchannels : 2) * (DEFDACBLKSIZE * sizeof(t_sample)));
    }
    if(cpd_sample_outs)
    {
        freebytes(cpd_sample_outs, (sys_outchannels ? sys_outchannels : 2) * (DEFDACBLKSIZE * sizeof(t_sample)));
    }
    if(c_first_instance)
    {
        pdinstance_free(c_first_instance);
    }
}




unsigned int cpd_version_getmajor()
{
    return PD_MAJOR_VERSION;
}

unsigned int cpd_version_getminor()
{
    return PD_MINOR_VERSION;
}

unsigned int cpd_version_getbug()
{
    return PD_BUGFIX_VERSION;
}





void cpd_searchpath_clear()
{
    namelist_free(sys_searchpath);
    sys_searchpath = NULL;
}

void cpd_searchpath_add(const char* path)
{
    sys_searchpath = namelist_append(sys_searchpath, path, 0);
}












static void cpd_print(const char* s)
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

void cpd_instance_set(cpd_instance* instance)
{
    pd_setinstance(instance->cpd_internal_ptr->c_intance);
    sys_soundin     = instance->cpd_internal_ptr->c_inputs;
    sys_soundout    = instance->cpd_internal_ptr->c_outputs;
    sys_inchannels  = instance->cpd_internal_ptr->c_ninputs;
    sys_outchannels = instance->cpd_internal_ptr->c_noutputs;
    sys_dacsr       = instance->cpd_internal_ptr->cpd_samplerate;
    c_current_instance = instance;
}

void cpd_instance_set_hook_console(cpd_instance* instance, cpd_hook_console* consolehook)
{
    instance->cpd_internal_ptr->c_m_normal  = consolehook->m_normal;
    instance->cpd_internal_ptr->c_m_log   = consolehook->m_log;
    instance->cpd_internal_ptr->c_m_error = consolehook->m_error;
    instance->cpd_internal_ptr->c_m_fatal = consolehook->m_fatal;
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

static c_receiver* cpd_instance_getreceiver(cpd_instance* instance, cpd_tie* tie)
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

void cpd_instance_bind(cpd_instance* instance, cpd_tie* tie, cpd_hook_message* messagehook)
{
    c_receiver *x = cpd_instance_getreceiver(instance, tie);
    if(x)
    {
        x->c_m_bang = messagehook->m_bang;
        x->c_m_float = messagehook->m_float;
        x->c_m_symbol = messagehook->m_symbol;
        x->c_m_gpointer = messagehook->m_gpointer;
        x->c_m_list = messagehook->m_list;
        x->c_m_anything = messagehook->m_anything;
    }
    else
    {
        x = (c_receiver *)pd_new(c_receiver_class);
        if(x)
        {
            x->c_sym = tie;
            x->c_owner = instance;
            x->c_m_bang = messagehook->m_bang;
            x->c_m_float = messagehook->m_float;
            x->c_m_symbol = messagehook->m_symbol;
            x->c_m_gpointer = messagehook->m_gpointer;
            x->c_m_list = messagehook->m_list;
            x->c_m_anything = messagehook->m_anything;
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

void cpd_instance_send_bang(cpd_instance* instance, cpd_tie const* tie)
{
    cpd_instance_set(instance);
    t_symbol const* sym = (t_symbol const *)tie;
    if(sym && sym->s_thing)
    {
        pd_bang((t_pd *)sym->s_thing);
    }
}

void cpd_instance_send_float(cpd_instance* instance, cpd_tie const* tie, cpd_float value)
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

void cpd_instance_send_list(cpd_instance* instance, cpd_tie const* tie, cpd_list const* list)
{
    cpd_instance_set(instance);
    t_symbol const* sym = (t_symbol const *)tie;
    if(sym && sym->s_thing)
    {
        pd_list((t_pd *)sym->s_thing, &s_list, list->l_n, list->l_vec);
    }
}

void cpd_instance_send_anything(cpd_instance* instance, cpd_tie const* tie, cpd_symbol const* symbol, cpd_list const* list)
{
    cpd_instance_set(instance);
    t_symbol const* sym = (t_symbol const *)tie;
    if(sym && sym->s_thing)
    {
        pd_typedmess((t_pd *)sym->s_thing, (t_symbol *)symbol, list->l_n, list->l_vec);
    }
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








cpd_patch* cpd_patch_new(const char* name, const char* path)
{
    int i;
    char* rpath;
    t_canvas* cnv = NULL;
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

void cpd_patch_free(cpd_patch* patch)
{
    canvas_free(patch);
}

const char* cpd_patch_get_name(cpd_patch const* patch)
{
    return patch->gl_name->s_name;
}

const char* cpd_patch_get_path(cpd_patch const* patch)
{
    return canvas_getdir((t_glist *)patch)->s_name;
}

int cpd_patch_get_dollarzero(cpd_patch const* patch)
{
    int value = 0;
    pd_pushsym((t_pd *)patch);
    value = canvas_getdollarzero();
    pd_popsym((t_pd *)patch);
    return value;
}

int cpd_patch_get_x(cpd_patch const* patch)
{
    return patch->gl_xmargin;
}

int cpd_patch_get_y(cpd_patch const* patch)
{
    return patch->gl_ymargin;
}

int cpd_patch_get_width(cpd_patch const* patch)
{
    return patch->gl_pixwidth;
}

int cpd_patch_get_height(cpd_patch const* patch)
{
    return patch->gl_pixheight;
}

cpd_object* cpd_patch_get_first_object(cpd_patch const* patch)
{
    return (cpd_object *)patch->gl_list;
}

cpd_object* cpd_patch_get_next_object(cpd_patch const* patch, cpd_object const* previous)
{
    return (cpd_object *)((t_gobj *)previous)->g_next;
}




static char cpd_object_is_patchable(cpd_object const* object)
{
    return object->te_g.g_pd->c_patchable;
}

static struct _widgetbehavior* cpd_object_get_widget(cpd_object const* object)
{
    return object->te_g.g_pd->c_wb;
}





cpd_symbol* cpd_object_get_name(cpd_object const* object)
{
    return object->te_g.g_pd->c_name;
}

void cpd_object_get_text(cpd_object const* object, int* size, char** text)
{
    binbuf_gettext(((t_text *)(object))->te_binbuf, text, size);
}

int cpd_object_get_x(cpd_object const* object, cpd_patch const* patch)
{
    int bounds[4];
    cpd_object_get_bounds(object, patch, bounds, bounds+1, bounds+2, bounds+3);
    return bounds[0];
}

int cpd_object_get_y(cpd_object const* object, cpd_patch const* patch)
{
    int bounds[4];
    cpd_object_get_bounds(object, patch, bounds, bounds+1, bounds+2, bounds+3);
    return bounds[1];
}

int cpd_object_get_width(cpd_object const* object, cpd_patch const* patch)
{
    int bounds[4];
    cpd_object_get_bounds(object, patch, bounds, bounds+1, bounds+2, bounds+3);
    return bounds[2];
}

int cpd_object_get_height(cpd_object const* object, cpd_patch const* patch)
{
    int bounds[4];
    cpd_object_get_bounds(object, patch, bounds, bounds+1, bounds+2, bounds+3);
    return bounds[3];
}

void cpd_object_get_bounds(cpd_object const* object, cpd_patch const* patch, int* x, int* y, int* width, int* height)
{
    struct _widgetbehavior const* wb = cpd_object_get_widget(object);
    if(cpd_object_is_patchable(object) && wb && wb->w_getrectfn)
    {
        wb->w_getrectfn((t_gobj *)object, (t_canvas *)patch, x, y, width, height);
        *width  = *width - *x;
        *height = *height - *y;
        *x = *x - cpd_patch_get_x(patch);
        *y = *y - cpd_patch_get_y(patch);
    }
    else
    {
        *x = 0;
        *y = 0;
        *width = 0;
        *height = 0;
    }
}




char cpd_object_is_gui(cpd_object const* object)
{
    t_symbol const* name = cpd_object_get_name(object);
    return name == c_sym_bng || name == c_sym_hsl || name == c_sym_vsl || name == c_sym_tgl || name == c_sym_nbx || name == c_sym_vradio || name == c_sym_hradio || name == c_sym_vu || name == c_sym_cnv;
}

cpd_symbol* cpd_gui_get_label(cpd_gui const* gui)
{
    return (c_sym_empty != gui->x_lab) ? gui->x_lab : &s_;
}

cpd_tie* cpd_gui_get_receive_tie(cpd_gui const* gui)
{
    return (c_sym_empty != gui->x_rcv) ? gui->x_rcv : &s_;
}

cpd_tie* cpd_gui_get_send_tie(cpd_gui const* gui)
{
    if(cpd_object_get_name((cpd_object const*)gui) != c_sym_vu)
    {
        return (c_sym_empty != gui->x_snd) ? gui->x_snd : &s_;
    }
    return &s_;
}

cpd_guitype cpd_gui_get_type(cpd_gui const* gui)
{
    t_symbol const* name = cpd_object_get_name((cpd_object const*)gui);
    if(name == c_sym_bng)
    {
        return CPD_GUI_BANG;
    }
    if(name == c_sym_hsl)
    {
        return CPD_GUI_SLIDERH;
    }
    if(name == c_sym_vsl)
    {
        return CPD_GUI_SLIDERV;
    }
    if(name == c_sym_tgl)
    {
        return CPD_GUI_TOGGLE;
    }
    if(name == c_sym_nbx)
    {
        return CPD_GUI_NUMBER;
    }
    if(name == c_sym_vradio)
    {
        return CPD_GUI_RADIOV;
    }
    if(name == c_sym_hradio)
    {
        return CPD_GUI_RADIOH;
    }
    if(name == c_sym_vu)
    {
        return CPD_GUI_VUMETER;
    }
    return CPD_GUI_PANEL;
}

float cpd_gui_get_maximum_value(cpd_gui const* gui)
{
    t_symbol const* name = cpd_object_get_name((cpd_object const*)gui);
    if(name == c_sym_hsl)
    {
        return ((t_hslider *)gui)->x_max;
    }
    else if(name == c_sym_vsl)
    {
        return ((t_vslider *)gui)->x_max;
    }
    else if(name == c_sym_tgl)
    {
        return 1.f;
    }
    else if(name == c_sym_nbx)
    {
        return ((t_my_numbox *)gui)->x_max;
    }
    else if(name == c_sym_vradio)
    {
        return ((t_hdial *)gui)->x_number - 1;
    }
    else if(name == c_sym_hradio)
    {
        return ((t_vdial *)gui)->x_number - 1;
    }
    return 0.f;
}

float cpd_gui_get_minimum_value(cpd_gui const* gui)
{
    t_symbol const* name = cpd_object_get_name((cpd_object const*)gui);
    if(name == c_sym_hsl)
    {
        return ((t_hslider *)gui)->x_min;
    }
    else if(name == c_sym_vsl)
    {
        return ((t_vslider *)gui)->x_min;
    }
    else if(name == c_sym_nbx)
    {
        return ((t_my_numbox *)gui)->x_min;
    }
    return 0.f;
}

int cpd_gui_get_number_of_steps(cpd_gui const* gui)
{
    t_symbol const* name = cpd_object_get_name((cpd_object const*)gui);
    if(name == c_sym_tgl)
    {
        return 2;
    }
    else if(name == c_sym_vradio)
    {
        return ((t_hdial *)gui)->x_number;
    }
    else if(name == c_sym_hradio)
    {
        return ((t_vdial *)gui)->x_number;
    }
    return 0;
}

float cpd_gui_get_value(cpd_gui const* gui)
{
    t_symbol const* name = cpd_object_get_name((cpd_object const*)gui);
    if(name == c_sym_hsl)
    {
        return ((t_hslider *)gui)->x_fval;
    }
    else if(name == c_sym_vsl)
    {
        return ((t_vslider *)gui)->x_fval;
    }
    else if(name == c_sym_tgl)
    {
        return ((t_toggle *)gui)->x_on;
    }
    else if(name == c_sym_nbx)
    {
        return ((t_my_numbox *)gui)->x_val;
    }
    else if(name == c_sym_vradio)
    {
        return ((t_hdial *)gui)->x_on;
    }
    else if(name == c_sym_hradio)
    {
        return ((t_vdial *)gui)->x_on;
    }
    return 0.f;
}

int cpd_gui_get_label_x(cpd_gui const* gui, cpd_patch const* patch)
{
    return cpd_object_get_x((cpd_object const*)gui, patch) + gui->x_ldx;
}

int cpd_gui_get_label_y(cpd_gui const* gui, cpd_patch const* patch)
{
    return cpd_object_get_y((cpd_object const*)gui, patch) + gui->x_ldy;
}





cpd_tie* cpd_tie_create(const char* name)
{
    return (cpd_tie *)gensym(name);
}

char const* cpd_tie_get_name(cpd_tie const* tie)
{
    return tie->s_name;
}

cpd_symbol* cpd_symbol_create(const char* symbol)
{
    return (cpd_symbol *)gensym(symbol);
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

cpd_atomtype cpd_list_get_type(cpd_list const* list, size_t index)
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

cpd_float cpd_list_get_float(cpd_list const* list, size_t index)
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

void cpd_list_set_float(cpd_list *list, size_t index, cpd_float value)
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

