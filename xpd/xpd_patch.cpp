/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_patch.hpp"
#include "PdObject.hpp"

extern "C"
{
#include "../cpd/cpd.h"
}

namespace xpd
{    
    // ==================================================================================== //
    //                                          PATCHER                                     //
    // ==================================================================================== //
    
    patch::patch(void* ptr, std::string const& name, std::string const& path, size_t uid) noexcept :
    m_ptr(ptr), m_name(name), m_path(path), m_unique_id(uid)
    {
        ;
    }
    
    patch::~patch() noexcept
    {
        ;
    }
    
    int patch::x() const noexcept
    {
        return cpd_patch_get_x(reinterpret_cast<c_patch const *>(m_ptr));
    }
    
    int patch::y() const noexcept
    {
        return cpd_patch_get_y(reinterpret_cast<c_patch const *>(m_ptr));
    }
    
    int patch::width() const noexcept
    {
        return cpd_patch_get_width(reinterpret_cast<c_patch const *>(m_ptr));
    }
    
    int patch::height() const noexcept
    {
        return cpd_patch_get_height(reinterpret_cast<c_patch const *>(m_ptr));
    }
    
    std::vector<Gui> patch::getGuis() const noexcept
    {
        std::vector<Gui> objects;
        if(isValid())
        {
            c_symbol* hsl = cpd_symbol_create("hsl");
            c_symbol* vsl = cpd_symbol_create("vsl");
            c_symbol* tgl = cpd_symbol_create("tgl");
            c_symbol* nbx = cpd_symbol_create("nbx");
            c_symbol* vra = cpd_symbol_create("vradio");
            c_symbol* hra = cpd_symbol_create("hradio");
            for(c_object *y = cpd_patch_get_first_object(reinterpret_cast<c_patch *>(m_ptr)); y;
                y = cpd_patch_get_next_object(reinterpret_cast<c_patch *>(m_ptr), y))
            {
                if(cpd_object_get_name(y) == hsl)
                {
                    objects.push_back(Gui(*this, Gui::Type::HorizontalSlider, reinterpret_cast<void *>(y)));
                }
                else if(cpd_object_get_name(y) == vsl)
                {
                    objects.push_back(Gui(*this, Gui::Type::VerticalSlider, reinterpret_cast<void *>(y)));
                }
                else if(cpd_object_get_name(y) == tgl)
                {
                    objects.push_back(Gui(*this, Gui::Type::Toggle, reinterpret_cast<void *>(y)));
                }
                else if(cpd_object_get_name(y) == nbx)
                {
                    objects.push_back(Gui(*this, Gui::Type::Number, reinterpret_cast<void *>(y)));
                }
                else if(cpd_object_get_name(y) == vra)
                {
                    objects.push_back(Gui(*this, Gui::Type::VerticalRadio, reinterpret_cast<void *>(y)));
                }
                else if(cpd_object_get_name(y) == hra)
                {
                    objects.push_back(Gui(*this, Gui::Type::HorizontalRadio, reinterpret_cast<void *>(y)));
                }
            }
        }
        return objects;
    }
    
    std::vector<Object> patch::getComments() const noexcept
    {
        std::vector<Object> objects;
        if(isValid())
        {
            c_symbol* txt = cpd_symbol_create("text");
            for(c_object *y = cpd_patch_get_first_object(reinterpret_cast<c_patch *>(m_ptr)); y;
                y = cpd_patch_get_next_object(reinterpret_cast<c_patch *>(m_ptr), y))
            {
                if(cpd_object_get_name(y)== txt)
                {
                    objects.push_back(Object(*this, reinterpret_cast<void *>(y)));
                }
            }
        }
        return objects;
    }
}



