/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "xpd_console.hpp"

namespace xpd
{
    
    console::history::history() :
    m_fatal_count(0),
    m_error_count(0),
    m_post_count(0),
    m_log_count(0)
    {
        m_messages.reserve(512);
    }
    
    size_t console::history::get_number_of_messages(level lvl) const noexcept
    {
        if(lvl == level::all)
        {
            return m_messages.size();
        }
        if(lvl == level::fatal)
        {
            return m_fatal_count;
        }
        if(lvl == level::error)
        {
            return m_error_count;
        }
        if(lvl == level::post)
        {
            return m_post_count;
        }
        return m_log_count;
    }
    
    size_t console::history::get_number_of_messages_until(level lvl) const noexcept
    {
        if(lvl == level::fatal)
        {
            return m_fatal_count;
        }
        if(lvl == level::error)
        {
            return m_fatal_count + m_error_count;
        }
        if(lvl == level::post)
        {
            return m_fatal_count + m_error_count + m_post_count;
        }
        return m_fatal_count + m_error_count + m_post_count + m_log_count;
    }
    
    console::message console::history::get_message(size_t index, level lvl) const
    {
        if(lvl == level::all)
        {
             return m_messages[index];
        }
        for(size_t i = 0, c = 0; i < m_messages.size(); ++i)
        {
            if(m_messages[i].lvl == lvl)
            {
                ++c;
                if(c == index+1)
                {
                    return m_messages[i];
                }
            }
        }
        return message{lvl, std::string()};
    }
    
    console::message console::history::get_message_until(size_t index, level lvl) const
    {
        for(size_t i = 0, c = 0; i < m_messages.size(); ++i)
        {
            if(m_messages[i].lvl <= lvl)
            {
                ++c;
                if(c == index+1)
                {
                    return m_messages[i];
                }
            }
        }
        return message{lvl, std::string()};
    }
    
    void console::history::clear() noexcept
    {
        m_fatal_count = m_error_count = m_post_count = m_log_count = 0;
        return m_messages.clear();
    }
    
    void console::history::add(message mess) noexcept
    {
        if(mess.lvl == level::fatal)
        {
            m_fatal_count++;
        }
        if(mess.lvl == level::error)
        {
            m_error_count++;
        }
        if(mess.lvl == level::post)
        {
            m_post_count++;
        }
        else
        {
            m_log_count++;
        }
        m_messages.push_back(std::move(mess));
    }
}