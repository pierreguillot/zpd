/*
// Copyright (c) 2015-2016 Pierre Guillot.
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
    
    void environment::initialize() xpd_noexcept
    {
        cpd_init();
    }
    
    void environment::clear() xpd_noexcept
    {
        cpd_clear();
    }
    
    unsigned int environment::version_major() xpd_noexcept
    {
        return cpd_version_getmajor();
    }
    
    unsigned int environment::version_minor() xpd_noexcept
    {
        return cpd_version_getminor();
    }
    
    unsigned int environment::version_bug() xpd_noexcept
    {
        return cpd_version_getbug();
    }
    
    void environment::searchpath_add(std::string const& path) xpd_noexcept
    {
        cpd_searchpath_add(path.c_str());
    }
    
    void environment::searpath_clear() xpd_noexcept
    {
        cpd_searchpath_clear();
    }
}

