/*
 // Copyright (c) 2015-2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "xpd_console.hpp"
#include <cassert>

namespace xpd
{
    
    console::history::history()
    {
        m_counters[0] = m_counters[1] = m_counters[2] = m_counters[3] = 0ul;
        m_posts.reserve(512);
    }
    
    size_t console::history::get_number_of_posts(level lvl) const xpd_noexcept
    {
        if(lvl == all)
        {
            return m_posts.size();
        }
        return m_counters[static_cast<size_t>(lvl)];
    }
    
    size_t console::history::get_number_of_posts_to_level(level lvl) const xpd_noexcept
    {
        if(lvl == all)
        {
            return m_posts.size();
        }
        size_t count = 0ul;
        for(size_t i = 0ul; i <= static_cast<size_t>(lvl); ++i)
        {
            count += m_counters[static_cast<size_t>(i)];
        }
        return count;
    }
    
    console::post console::history::get_post(size_t index, level lvl) const
    {
        if(lvl == all)
        {
             return m_posts[index];
        }
        for(size_t i = 0, c = 0; i < m_posts.size(); ++i)
        {
            if(m_posts[i].type == lvl)
            {
                ++c;
                if(c == index+1)
                {
                    return m_posts[static_cast<size_t>(i)];
                }
            }
        }
#define LCOV_EXCL_START
        assert("The post index is out of bounds" && 0);
#define LCOV_EXCL_STOP
    }
    
    console::post console::history::get_post_to_level(size_t index, level lvl) const
    {
        for(size_t i = 0, c = 0; i < m_posts.size(); ++i)
        {
            if(m_posts[i].type <= lvl)
            {
                ++c;
                if(c == index+1)
                {
                    return m_posts[static_cast<size_t>(i)];
                }
            }
        }
#define LCOV_EXCL_START
        assert("The post index is out of bounds" && 0);
#define LCOV_EXCL_STOP
    }
    
    void console::history::clear() xpd_noexcept
    {
        m_counters[0] = m_counters[1] = m_counters[2] = m_counters[3] = 0ul;
        return m_posts.clear();
    }
    
    void console::history::add(post const& mess) xpd_noexcept
    {
        m_posts.push_back(mess);
        assert("The post type can only be fatal, error, normal or log" && mess.type <= log);
        m_counters[static_cast<size_t>(mess.type)]++;
    }
}