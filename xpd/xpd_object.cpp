/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_object.hpp"


extern "C"
{
#include "../cpd/cpd.h"
}


namespace xpd
{
    std::string object::name() const
    {
        return std::string(cpd_symbol_get_name(cpd_object_get_name(reinterpret_cast<c_object const*>(m_ptr))));
    }
    
    std::string object::text() const
    {
        int size = 0;
        char* text = NULL;
        cpd_object_get_text(reinterpret_cast<c_object const*>(m_ptr), &size, &text);
        if(size && text)
        {
            std::string txt(text);
            free(text);
            return txt;
        }
        return std::string();
    }
    
    int object::x() const xpd_noexcept
    {
        int bounds[4];
        cpd_object_get_bounds(reinterpret_cast<c_object *>(m_ptr), reinterpret_cast<c_patch const*>(m_patch),
                              bounds, bounds+1, bounds+2, bounds+3);
        return bounds[0];
    }
    
    int object::y() const xpd_noexcept
    {
        int bounds[4];
        cpd_object_get_bounds(reinterpret_cast<c_object *>(m_ptr), reinterpret_cast<c_patch const*>(m_patch),
                              bounds, bounds+1, bounds+2, bounds+3);
        return bounds[1];
    }
    
    int object::width() const xpd_noexcept
    {
        int bounds[4];
        cpd_object_get_bounds(reinterpret_cast<c_object *>(m_ptr), reinterpret_cast<c_patch const*>(m_patch),
                              bounds, bounds+1, bounds+2, bounds+3);
        return bounds[2];
    }
    
    int object::height() const xpd_noexcept
    {
        int bounds[4];
        cpd_object_get_bounds(reinterpret_cast<c_object *>(m_ptr), reinterpret_cast<c_patch const*>(m_patch),
                              bounds, bounds+1, bounds+2, bounds+3);
        return bounds[3];
    }
}



