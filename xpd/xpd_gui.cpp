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
    
    symbol gui::receive_symbol() const xpd_noexcept
    {
        return symbol(cpd_symbol_get_name(cpd_gui_get_receive_symbol(reinterpret_cast<c_gui const*>(m_ptr))));
    }
    
    symbol gui::send_symbol() const xpd_noexcept
    {
        return symbol(cpd_symbol_get_name(cpd_gui_get_send_symbol(reinterpret_cast<c_gui const*>(m_ptr))));
    }
    
    symbol gui::label() const xpd_noexcept
    {
        return symbol(cpd_symbol_get_name(cpd_gui_get_label(reinterpret_cast<c_gui const*>(m_ptr))));
    }
    
    gui::type_t gui::type() const xpd_noexcept
    {
        return type_t(cpd_gui_get_type(reinterpret_cast<c_gui const*>(m_ptr)));
    }
}



