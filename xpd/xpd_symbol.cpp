/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_symbol.hpp"

extern "C"
{
#include "../zpd/z_pd.h"
}


namespace xpd
{
    // ==================================================================================== //
    //                                      SYMBOL                                          //
    // ==================================================================================== //
    
    symbol::symbol(std::string const& name) : ptr(z_pd_tie_create(name.c_str()))
    {
        if(!ptr)
        {
            throw std::runtime_error("can't allocate symbol.");
        }
    }
    
    symbol::symbol(char const* name) : ptr(z_pd_tie_create(name))
    {
        if(!ptr)
        {
            throw std::runtime_error("can't allocate symbol.");
        }
    }
    
    symbol& symbol::operator=(std::string const& name)
    {
        ptr = z_pd_tie_create(name.c_str());
        if(!ptr)
        {
            throw std::runtime_error("can't allocate symbol.");
        }
        return *this;
    }
    
    symbol& symbol::operator=(char const* name)
    {
        ptr = z_pd_tie_create(name);
        if(!ptr)
        {
            throw std::runtime_error("can't allocate symbol.");
        }
        return *this;
    }
    
    std::string symbol::name() const
    {
        return z_pd_tie_get_name(reinterpret_cast<z_tie const *>(ptr));
    }
}

