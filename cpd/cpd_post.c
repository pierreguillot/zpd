/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.


#include "cpd_post.h"
#include "../pd/src/m_pd.h"
#include "../pd/src/s_stuff.h"
#include <stdlib.h>


void cpd_instance_lock_hook_console(cpd_instance* instance, cpd_hook_console* consolehook)
{
    instance->cpd_internal_ptr->c_console = *consolehook;
}


void cpd_instance_post_fatal(cpd_instance* instance, char const* message, ...)
{
    if(instance->cpd_internal_ptr->c_console.m_fatal)
    {
        char buf[MAXPDSTRING];
        va_list ap;
        va_start(ap, message);
        vsnprintf(buf, MAXPDSTRING-1, message, ap);
        va_end(ap);
        instance->cpd_internal_ptr->c_console.m_fatal(instance, buf);
    }
}

void cpd_instance_post_error(cpd_instance* instance, char const* message, ...)
{
    if(instance->cpd_internal_ptr->c_console.m_error)
    {
        char buf[MAXPDSTRING];
        va_list ap;
        va_start(ap, message);
        vsnprintf(buf, MAXPDSTRING-1, message, ap);
        va_end(ap);
        instance->cpd_internal_ptr->c_console.m_error(instance, buf);
    }
}

void cpd_instance_post_normal(cpd_instance* instance, char const* message, ...)
{
    if(instance->cpd_internal_ptr->c_console.m_normal)
    {
        char buf[MAXPDSTRING];
        va_list ap;
        va_start(ap, message);
        vsnprintf(buf, MAXPDSTRING-1, message, ap);
        va_end(ap);
        instance->cpd_internal_ptr->c_console.m_normal(instance, buf);
    }
}

void cpd_instance_post_log(cpd_instance* instance, char const* message, ...)
{
    if(instance->cpd_internal_ptr->c_console.m_log)
    {
        char buf[MAXPDSTRING];
        va_list ap;
        va_start(ap, message);
        vsnprintf(buf, MAXPDSTRING-1, message, ap);
        va_end(ap);
        instance->cpd_internal_ptr->c_console.m_log(instance, buf);
    }
}





