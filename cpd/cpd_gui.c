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
extern cpd_symbol*        c_sym_empty;

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

















