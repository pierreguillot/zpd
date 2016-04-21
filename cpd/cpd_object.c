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


extern cpd_symbol*        c_sym_bng;
extern cpd_symbol*        c_sym_hsl;
extern cpd_symbol*        c_sym_vsl;
extern cpd_symbol*        c_sym_tgl;
extern cpd_symbol*        c_sym_nbx;
extern cpd_symbol*        c_sym_vradio;
extern cpd_symbol*        c_sym_hradio;
extern cpd_symbol*        c_sym_vu;
extern cpd_symbol*        c_sym_cnv;

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

static void cpd_object_get_bounds(cpd_object const* object, cpd_patch const* patch, int* x, int* y, int* width, int* height)
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

char cpd_object_is_gui(cpd_object const* object)
{
    t_symbol const* name = cpd_object_get_name(object);
    return name == c_sym_bng || name == c_sym_hsl || name == c_sym_vsl || name == c_sym_tgl || name == c_sym_nbx || name == c_sym_vradio || name == c_sym_hradio || name == c_sym_vu || name == c_sym_cnv;
}

















