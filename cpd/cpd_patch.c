/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#include "cpd_patch.h"
#include "../pd/src/m_pd.h"
#include "../pd/src/m_imp.h"
#include "../pd/src/s_stuff.h"
#include "../pd/src/g_canvas.h"
#include "../pd/src/g_all_guis.h"

extern void cpd_instance_lock(cpd_instance* instance);
extern void cpd_instance_unlock(cpd_instance* instance);

cpd_patch* cpd_instance_patch_load(cpd_instance* instance, const char* name, const char* path)
{
    int i;
    char* rpath;
    t_canvas* cnv = NULL;
    cpd_instance_lock(instance);
    if(name && path)
    {
        cnv = (t_canvas *)glob_evalfile(NULL, gensym(name), gensym(path));
        if(cnv)
        {
            cpd_instance_unlock(instance);
            return cnv;
        }
    }
    else if(name)
    {
        
        i = 0;
        while((rpath = namelist_get(STUFF->st_searchpath, i)) != NULL)
        {
            cnv = (t_canvas *)glob_evalfile(NULL, gensym(name), gensym(rpath));
            if(cnv)
            {
                cpd_instance_unlock(instance);
                return cnv;
            }
            i++;
        }
    }
    cpd_instance_unlock(instance);
    return NULL;
}

void cpd_instance_patch_close(cpd_instance* instance, cpd_patch* patch)
{
    cpd_instance_lock(instance);
    canvas_free(patch);
    cpd_instance_unlock(instance);
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













