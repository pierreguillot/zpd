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
    
    
    std::string environment::font_name()
    {
        return std::string(cpd_get_font_name());
    }
    
    std::string environment::font_weight()
    {
        return std::string(cpd_get_font_weight());
    }
    
    unsigned int environment::font_size()
    {
        return cpd_get_font_size();
    }
}

