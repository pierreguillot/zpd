/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_tie.hpp"

extern "C"
{
#include "../cpd/cpd.h"
}


namespace xpd
{
    // ==================================================================================== //
    //                                      TIE                                             //
    // ==================================================================================== //
    
    tie::tie(std::string const& name) : ptr(cpd_tie_create(name.c_str()))
    {
        if(!ptr)
        {
            throw std::runtime_error("can't allocate tie.");
        }
    }
    
    tie::tie(char const* name) : ptr(cpd_tie_create(name))
    {
        if(!ptr)
        {
            throw std::runtime_error("can't allocate tie.");
        }
    }
    
    tie& tie::operator=(std::string const& name)
    {
        ptr = cpd_tie_create(name.c_str());
        if(!ptr)
        {
            throw std::runtime_error("can't allocate tie.");
        }
        return *this;
    }
    
    tie& tie::operator=(char const* name)
    {
        ptr = cpd_tie_create(name);
        if(!ptr)
        {
            throw std::runtime_error("can't allocate tie.");
        }
        return *this;
    }
    
    std::string tie::name() const
    {
        return cpd_tie_get_name(reinterpret_cast<c_tie const *>(ptr));
    }
}

