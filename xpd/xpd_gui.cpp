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
}



