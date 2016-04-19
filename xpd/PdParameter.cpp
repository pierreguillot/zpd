/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "PdParameter.hpp"


namespace xpd
{
    /*
    Parameter::Parameter()
    : m_valid(false), m_value (0.f), m_min(0.f), m_max(0.f),
    m_name (""), m_label(""), m_bname(), m_nsteps(0)
    {
        
    }
    
    Parameter::Parameter(Parameter const& other)
    : m_valid(other.m_valid), m_value (other.m_value),
    m_min(other.m_min), m_max(other.m_max),
    m_name (other.m_name), m_label(other.m_label),
    m_bname(other.m_bname), m_nsteps(other.m_nsteps)
    {
        
    }
    
    Parameter::Parameter(xpd::Gui const& gui)
    : m_valid(true), m_value (0.f),
    m_min(gui.getMinimum()),
    m_max(gui.getMaximum()),
    m_name(gui.getName()),
    m_label(gui.getLabel()),
    m_bname(gui.getReceivetie()),
    m_nsteps(int(gui.getNumberOfSteps()))
    {
        setValueNonNormalized(gui.getValue());
    }
    
    Parameter::~Parameter()
    {
        
    }
    
    Parameter& Parameter::operator=(Parameter const& other)
    {
        m_valid = other.m_valid;
        m_value = other.m_value;
        m_min   = other.m_min;
        m_max   = other.m_max;
        m_name  = other.m_name;
        m_label = other.m_label;
        m_bname = other.m_bname;
        m_nsteps= other.m_nsteps;
        return *this;
    }
    
    Parameter& Parameter::operator=(Parameter&& other)
    {
        std::swap(m_valid, other.m_valid);
        std::swap(m_value, other.m_value);
        std::swap(m_min, other.m_min);
        std::swap(m_max, other.m_max);
        std::swap(m_name, other.m_name);
        std::swap(m_label, other.m_label);
        std::swap(m_bname, other.m_bname);
        std::swap(m_nsteps, other.m_nsteps);
        return *this;
    }
    
    void Parameter::setValueNormalized(float value)
    {
        value = std::min(std::max(value, 0.f), 1.f);
        if(m_nsteps)
        {
            const float step = (1.f/ float(m_nsteps - 1));
            m_value = std::round(value / step) * step;
        }
        else
        {
            m_value = value;
        }
    }
    
    
    float Parameter::getValueNormalized(float value)
    {
        if(m_min < m_max)
        {
            return std::min(std::max((value - m_min) / (m_max - m_min), 0.f), 1.f);
        }
        return std::min(std::max(1.f  - ((value - m_max) / (m_min - m_max)), 0.f), 1.f);
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
     */
}