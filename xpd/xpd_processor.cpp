/*
 // Copyright (c) 2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_processor.hpp"
#include <cassert>
#include <cmath>

namespace xpd
{
    symbol processor::get_symbol_float()
    {
        static symbol x_sym("float");
        return x_sym;
    }
    
    symbol processor::get_symbol_list()
    {
        static symbol x_sym("list");
        return x_sym;
    }
    
    // ================================================================================ //
    //                                      DEFAULT                                     //
    // ================================================================================ //
    
    
    processor::processor() : m_playhead_list(14)
    {
        get_symbol_float();
        get_symbol_list();
    }
    
    processor::~processor()
    {
        if(static_cast<bool>(m_patch))
        {
            close(m_patch);
            m_playhead_tie = tie();
        }
    }
    
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    void processor::receive(tie name, symbol selector, std::vector<atom> const& atoms)
    {
        if(!atoms.empty() && selector == get_symbol_float() && atoms[0].type() == atom::float_t)
        {
            for(size_t i = 0; i < m_parameters.size(); ++i)
            {
                if(m_parameters[i].receive_tie == name)
                {
                    set_parameter_value(i, static_cast<float>(atoms[i]), false);
                    return;
                }
            }
        }
    }
    
    void processor::receive(console::post const& post)
    {
        console::history::add(std::move(post));
    }
    
    // ================================================================================ //
    //                                      PATCH                                       //
    // ================================================================================ //
    
    void processor::load_patch(const std::string &name, const std::string &path)
    {
        processor::release();
        if(static_cast<bool>(m_patch))
        {
            close(m_patch);
        }
        m_patch = processor::load(name, path);
        if(static_cast<bool>(m_patch))
        {
            m_playhead_tie = tie(std::to_string(m_patch.unique_id()) + "-playhead");
        }
        else
        {
            send(console::post(console::error, std::string("processor can't find the patch : ") + name));
        }
    }
    
    
    void processor::close_patch()
    {
        processor::release();
        if(static_cast<bool>(m_patch))
        {
            close(m_patch);
            m_playhead_tie = tie();
        }
    }
    
    // ================================================================================ //
    //                                      PARAMETERS                                  //
    // ================================================================================ //
    
    void processor::clear_parameters()
    {
        for(size_t i = 0; i < m_parameters.size(); ++i)
        {
            processor::unbind(m_parameters[i].send_tie);
        }
        m_parameters.clear();
    }
    
    void processor::add_parameter(parameter const& param)
    {
        assert(!param.name.empty() && "the name of a processor parameter can't be empty.");
        assert(static_cast<bool>(param.receive_tie) && "the receive tie of a processor parameter can't be empty.");
        assert(static_cast<bool>(param.send_tie) && "the send tie of a processor parameter can't be empty.");
        for(size_t i = 0; i < m_parameters.size(); ++i)
        {
            if(m_parameters[i].name == param.name ||
               m_parameters[i].receive_tie == param.receive_tie ||
               m_parameters[i].send_tie == param.send_tie)
            {
                throw "processor receives a duplicated parameter.";
            }
        }
        m_parameters.push_back(param);
        processor::bind(param.send_tie);
    }
    
    float processor::get_parameter_default_value(size_t const index, bool const normalized) const xpd_noexcept
    {
        return normalized ? m_parameters[index].defval : m_parameters[index].defval * (m_parameters[index].max - m_parameters[index].min) + m_parameters[index].min;
    }
    
    float processor::get_parameter_value(size_t const index, bool const normalized) const xpd_noexcept
    {
        return normalized ? m_parameters[index].value : m_parameters[index].value * (m_parameters[index].max - m_parameters[index].min) + m_parameters[index].min;
    }
    
    void processor::set_parameter_value(size_t const index, float val, bool const normalized)
    {
        if(!normalized)
        {
            val = (val - m_parameters[index].min) / (m_parameters[index].max - m_parameters[index].min);
        }
        if(m_parameters[index].nsteps > 1)
        {
            const float step = (1.f / float(m_parameters[index].nsteps - 1));
            val = std::round(val / step) * step;
        }
        m_parameters[index].value = val;
        processor::send(m_parameters[index].send_tie, get_symbol_float(), {get_parameter_value(index, false)});
    }
    
    
    // ================================================================================ //
    //                                      LISTENER                                    //
    // ================================================================================ //

    void processor::add_listener(listener& l)
    {
        m_listeners.insert(&l);
    }
    
    void processor::remove_listener(listener& l)
    {
        m_listeners.erase(&l);
    }
}

