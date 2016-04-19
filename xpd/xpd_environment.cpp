/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_environment.hpp"

extern "C"
{
#include "../cpd/cpd.h"
}


namespace xpd
{
    // ==================================================================================== //
    //                                      PD                                              //
    // ==================================================================================== //
    
    environment::environment() noexcept
    {
        cpd_init();
    }
    
    environment::~environment() noexcept
    {
        cpd_clear();
    }
    
    environment& environment::get() noexcept
    {
        static environment pd;
        return pd;
    }
    
    unsigned int environment::version_major() noexcept
    {
        return cpd_version_getmajor();
    }
    
    unsigned int environment::version_minor() noexcept
    {
        return cpd_version_getminor();
    }
    
    unsigned int environment::version_bug() noexcept
    {
        return cpd_version_getbug();
    }
    
    void environment::searchpath_add(std::string const& path) noexcept
    {
        std::lock_guard<std::mutex> guard(environment::get().m_mutex);
        cpd_searchpath_add(path.c_str());
    }
    
    void environment::searpath_clear() noexcept
    {
        std::lock_guard<std::mutex> guard(environment::get().m_mutex);
        cpd_searchpath_clear();
    }
    
    void environment::lock() noexcept
    {
        environment::get().m_mutex.lock();
    }
    
    void environment::unlock() noexcept
    {
        environment::get().m_mutex.unlock();
    }
}

