/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_gui.hpp"

extern "C"
{
#include "../cpd/cpd.h"
}


namespace xpd
{
    gui::gui(object const& other) : object(other)
    {
        if(!cpd_object_is_gui(reinterpret_cast<c_object const*>(m_ptr)))
        {
            throw "The object isn't a gui.";
        }
    }
    
    gui& gui::operator=(object const& other)
    {
        object::operator=(other);
        if(!cpd_object_is_gui(reinterpret_cast<c_object const*>(m_ptr)))
        {
            throw "The object isn't a gui.";
        }
        return *this;
    }
    
    gui::type_t gui::type() const xpd_noexcept
    {
        return type_t(cpd_gui_get_type(reinterpret_cast<c_gui const*>(m_ptr)));
    }
    
    symbol gui::receive_tie() const xpd_noexcept
    {
        return symbol(cpd_tie_get_name(cpd_gui_get_receive_tie(reinterpret_cast<c_gui const*>(m_ptr))));
    }
    
    symbol gui::send_tie() const xpd_noexcept
    {
        return symbol(cpd_tie_get_name(cpd_gui_get_send_tie(reinterpret_cast<c_gui const*>(m_ptr))));
    }
    
    symbol gui::label() const xpd_noexcept
    {
        return symbol(cpd_symbol_get_name(cpd_gui_get_label(reinterpret_cast<c_gui const*>(m_ptr))));
    }
    
    int gui::label_x() const xpd_noexcept
    {
        return cpd_gui_get_label_x(reinterpret_cast<c_gui const*>(m_ptr), reinterpret_cast<c_patch const*>(m_patch));
    }
    
    int gui::label_y() const xpd_noexcept
    {
        return cpd_gui_get_label_y(reinterpret_cast<c_gui const*>(m_ptr), reinterpret_cast<c_patch const*>(m_patch));
    }
    
    float gui::minimum() const xpd_noexcept
    {
        return cpd_gui_get_minimum_value(reinterpret_cast<c_gui const*>(m_ptr));
    }
    
    float gui::maximum() const xpd_noexcept
    {
        return cpd_gui_get_maximum_value(reinterpret_cast<c_gui const*>(m_ptr));
    }
    
    float gui::value() const xpd_noexcept
    {
        return cpd_gui_get_value(reinterpret_cast<c_gui const*>(m_ptr));
    }
    
    size_t gui::nsteps() const xpd_noexcept
    {
        return cpd_gui_get_number_of_steps(reinterpret_cast<c_gui const*>(m_ptr));
    }
}



