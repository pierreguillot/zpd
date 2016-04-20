/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_patch.hpp"
#include "xpd_object.hpp"

extern "C"
{
#include "../cpd/cpd.h"
}

namespace xpd
{    
    // ==================================================================================== //
    //                                          PATCHER                                     //
    // ==================================================================================== //
    
    patch::patch(void* ptr, size_t uid) xpd_noexcept : m_ptr(ptr), m_unique_id(uid)
    {
        ;
    }
    
    patch::~patch() xpd_noexcept
    {
        ;
    }
    
    std::string patch::name() const
    {
        return std::string(cpd_patch_get_name(reinterpret_cast<c_patch const *>(m_ptr)));
    }
    
    std::string patch::path() const
    {
        return std::string(cpd_patch_get_path(reinterpret_cast<c_patch const *>(m_ptr)));
    }
    
    int patch::x() const xpd_noexcept
    {
        return cpd_patch_get_x(reinterpret_cast<c_patch const *>(m_ptr));
    }
    
    int patch::y() const xpd_noexcept
    {
        return cpd_patch_get_y(reinterpret_cast<c_patch const *>(m_ptr));
    }
    
    int patch::width() const xpd_noexcept
    {
        return cpd_patch_get_width(reinterpret_cast<c_patch const *>(m_ptr));
    }
    
    int patch::height() const xpd_noexcept
    {
        return cpd_patch_get_height(reinterpret_cast<c_patch const *>(m_ptr));
    }
    
    std::vector<object> patch::objects() const xpd_noexcept
    {
        std::vector<object> objects;
        for(c_object *y = cpd_patch_get_first_object(reinterpret_cast<c_patch *>(m_ptr)); y;
            y = cpd_patch_get_next_object(reinterpret_cast<c_patch *>(m_ptr), y))
        {
            objects.push_back(object(m_ptr, y));
        }
        return objects;
    }
}



