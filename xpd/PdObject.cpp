/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "PdObject.hpp"


extern "C"
{
#include "../cpd/cpd.h"
}


namespace xpd
{
    
    // ==================================================================================== //
    //                                      OBJECT                                          //
    // ==================================================================================== //
    
    object::object(patch const* patch, void* ptr) noexcept
    {
        ;
    }
    
    // ==================================================================================== //
    //                                      GUI                                          //
    // ==================================================================================== //
    
    Gui::Gui(patch const& patch, Type type, void* ptr) noexcept : object(patch, ptr), m_type(type)
    {
        ;
    }
    
    Gui::~Gui() noexcept
    {
        ;
    }
    
    Gui::Type Gui::getType() const noexcept
    {
        return m_type;
    }
    
    bool Gui::isParameter() const noexcept
    {
        return isValid() && !getName().empty() && getReceivetie() != tie();
    }
    
    std::string Gui::getName() const
    {
        if(isValid())
        {
            c_symbol* s = cpd_gui_get_label(reinterpret_cast<c_gui *>(getPtr()));
            if(s)
            {
                std::string name(cpd_symbol_get_name(s));
                if(!name.empty() && name != "empty")
                {
                    auto pos = name.find("_");
                    if(pos != std::string::npos)
                    {
                        name.erase(name.begin()+pos, name.end());
                    }
                    return name;
                }
            }
        }
        return std::string();
    }
    
    std::string Gui::getLabel() const
    {
        if(isValid())
        {
            c_symbol* s = cpd_gui_get_label(reinterpret_cast<c_gui *>(getPtr()));
            if(s)
            {
                std::string name(cpd_symbol_get_name(s));
                if(!name.empty() && name != "empty")
                {
                    auto pos = name.find("_");
                    if(pos != std::string::npos)
                    {
                        name.erase(name.begin(), name.begin()+pos+1);
                        return name;
                    }
                }
            }            
        }
        return std::string();
    }
    
    
    tie Gui::getReceivetie() const
    {
        if(isValid())
        {
            c_symbol* s = cpd_gui_get_receive_symbol(reinterpret_cast<c_gui *>(getPtr()));
            if(s && s != cpd_symbol_create("empty"))
            {
                return smuggler::createtie(s);
            }
        }
        return tie();
    }
    
    size_t Gui::getNumberOfSteps() const noexcept
    {
        if(isValid())
        {
            return cpd_gui_get_number_of_steps(reinterpret_cast<c_gui *>(getPtr()));
        }
        return 0.f;
    }
    
    float Gui::getMinimum() const noexcept
    {
        if(isValid())
        {
            return cpd_gui_get_minimum_value(reinterpret_cast<c_gui *>(getPtr()));
        }
        return 0.f;
    }
    
    float Gui::getMaximum() const noexcept
    {
        if(isValid())
        {
            return cpd_gui_get_maximum_value(reinterpret_cast<c_gui *>(getPtr()));
        }
        return 1.f;
    }
    
    float Gui::getValue() const noexcept
    {
        if(isValid())
        {
            return cpd_gui_get_value(reinterpret_cast<c_gui *>(getPtr()));
        }
        return 0.f;
    }
    
    Point<int> Gui::getLabelPosition() const noexcept
    {
        int x = 0, y = 0;
        if(isValid())
        {
            cpd_gui_get_label_position(reinterpret_cast<c_gui *>(getPtr()),
                                reinterpret_cast<c_patch *>(getpatchPtr()),
                                &x, &y);
        }
        return {x, y};
    }
}



