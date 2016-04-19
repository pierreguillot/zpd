/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_symbol.hpp"
#include <exception>

extern "C"
{
#include "../cpd/cpd.h"
}


namespace xpd
{    
    // ==================================================================================== //
    //                                      SYMBOL                                          //
    // ==================================================================================== //
    
    symbol const symbol::bang_s   = symbol("bang");
    symbol const symbol::float_s  = symbol("float");
    symbol const symbol::symbol_s = symbol("symbol");
    symbol const symbol::list_s   = symbol("list");
    
    symbol::symbol(std::string const& name) : ptr(cpd_tie_create(name.c_str()))
    {
        if(!ptr)
        {
            throw "can't allocate symbol.";
        }
    }
    
    symbol::symbol(char const* name) : ptr(cpd_tie_create(name))
    {
        if(!ptr)
        {
            throw "can't allocate symbol.";
        }
    }
    
    symbol& symbol::operator=(std::string const& name)
    {
        ptr = cpd_tie_create(name.c_str());
        if(!ptr)
        {
            throw "can't allocate symbol.";
        }
        return *this;
    }
    
    symbol& symbol::operator=(char const* name)
    {
        ptr = cpd_tie_create(name);
        if(!ptr)
        {
            throw "can't allocate symbol.";
        }
        return *this;
    }
    
    std::string symbol::name() const
    {
        return cpd_tie_get_name(reinterpret_cast<c_tie const *>(ptr));
    }
}

