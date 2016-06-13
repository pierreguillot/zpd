/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#include "cpd_environment.h"
#include "cpd_types.h"
#include "cpd_instance.h"

#include "../pd/src/m_pd.h"
#include "../pd/src/s_stuff.h"
#include "../pd/src/m_imp.h"
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// ==================================================================================== //
//                                      GENERAL                                         //
// ==================================================================================== //

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








// ==================================================================================== //
//                                   IMPLEMENTATION                                     //
// ==================================================================================== //

#ifdef _WIN32
#include <windows.h>
static CRITICAL_SECTION  c_mutex;
#else
#include <pthread.h>
static pthread_mutex_t c_mutex;
#endif

extern void cpd_lock()
{
#ifdef _WIN32
    EnterCriticalSection(&c_mutex);
#else
    pthread_mutex_lock(&c_mutex);
#endif
}

extern void cpd_unlock()
{
#ifdef _WIN32
    LeaveCriticalSection(&c_mutex);
#else
    pthread_mutex_unlock(&c_mutex);
#endif
}



static t_sample*          c_sample_ins    = NULL;
static t_sample*          c_sample_outs   = NULL;
static t_pdinstance*      c_first_instance = NULL;
cpd_symbol*        c_sym_bng           = NULL;
cpd_symbol*        c_sym_hsl           = NULL;
cpd_symbol*        c_sym_vsl           = NULL;
cpd_symbol*        c_sym_tgl           = NULL;
cpd_symbol*        c_sym_nbx           = NULL;
cpd_symbol*        c_sym_vradio        = NULL;
cpd_symbol*        c_sym_hradio        = NULL;
cpd_symbol*        c_sym_vu            = NULL;
cpd_symbol*        c_sym_cnv           = NULL;
cpd_symbol*        c_sym_empty         = NULL;

extern void cpd_print(const char* s);
extern cpd_instance* c_current_instance;

// ==================================================================================== //
//                                      INTERFACE                                       //
// ==================================================================================== //

void cpd_init()
{
    int devices = 0;
    int ioputs  = 2;
    static char initialized = 0;
    assert("Pure Data is already initialized." && !initialized);
    if(!initialized)
    {
        sys_soundin         = NULL;
        sys_soundout        = NULL;
        c_current_instance  = NULL;
        sys_printhook = (t_printhook)(cpd_print);
        
#ifdef _WIN32
        InitializeCriticalSection(&c_mutex);
#else
        pthread_mutex_init(&c_mutex, NULL);
#endif
        signal(SIGFPE, SIG_IGN);
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
        
        c_sample_ins      = sys_soundin;
        c_sample_outs     = sys_soundout;
        c_first_instance    = pd_this;
        
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

        initialized = 1;
    }
}

void cpd_clear()
{
    if(c_sample_ins)
    {
        freebytes(c_sample_ins, (sys_inchannels ? sys_inchannels : 2) * (DEFDACBLKSIZE * sizeof(t_sample)));
    }
    if(c_sample_outs)
    {
        freebytes(c_sample_outs, (sys_outchannels ? sys_outchannels : 2) * (DEFDACBLKSIZE * sizeof(t_sample)));
    }
    if(c_first_instance)
    {
        pdinstance_free(c_first_instance);
    }
#ifdef _WIN32
    DeleteCriticalSection(&c_mutex);
#else
    pthread_mutex_destroy(&c_mutex);
#endif
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





